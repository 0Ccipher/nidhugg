/* Copyright (C) 2014-2017 Carl Leonardsson
 *
 * This file is part of Nidhugg.
 *
 * Nidhugg is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nidhugg is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "Debug.h"
#include "CMInterpreter.h"

#if defined(HAVE_LLVM_IR_LLVMCONTEXT_H)
#include <llvm/IR/LLVMContext.h>
#elif defined(HAVE_LLVM_LLVMCONTEXT_H)
#include <llvm/LLVMContext.h>
#endif

static void SetValue(llvm::Value *V, llvm::GenericValue Val, llvm::ExecutionContext &SF) {
  SF.Values[V] = Val;
}

CMInterpreter::CMInterpreter(llvm::Module *M, CMTraceBuilder &TB,
                               const Configuration &conf)
  : Interpreter(M,TB,conf) {
  CM_threads.push_back(CMThread());
}

CMInterpreter::~CMInterpreter(){
}

std::unique_ptr<CMInterpreter> CMInterpreter::
create(llvm::Module *M, CMTraceBuilder &TB, const Configuration &conf,
       std::string *ErrorStr){
#ifdef LLVM_MODULE_MATERIALIZE_ALL_PERMANENTLY_ERRORCODE_BOOL
  if(std::error_code EC = M->materializeAllPermanently()){
    // We got an error, just return 0
    if(ErrorStr) *ErrorStr = EC.message();
    return 0;
  }
#elif defined LLVM_MODULE_MATERIALIZE_ALL_PERMANENTLY_BOOL_STRPTR
  if (M->MaterializeAllPermanently(ErrorStr)){
    // We got an error, just return 0
    return 0;
  }
#elif defined LLVM_MODULE_MATERIALIZE_LLVM_ALL_ERROR
  if (llvm::Error Err = M->materializeAll()) {
    std::string Msg;
    handleAllErrors(std::move(Err), [&](llvm::ErrorInfoBase &EIB) {
      Msg = EIB.message();
    });
    if (ErrorStr)
      *ErrorStr = Msg;
    // We got an error, just return 0
    return nullptr;
  }
#else
  if(std::error_code EC = M->materializeAll()){
    // We got an error, just return 0
    if(ErrorStr) *ErrorStr = EC.message();
    return 0;
  }
#endif

  return std::unique_ptr<CMInterpreter>(new CMInterpreter(M,TB,conf));
}

void CMInterpreter::runAux(int proc, int aux){
  /* Perform an update from store buffer to memory. */

  assert(aux == 0);
  assert(CM_threads[proc].store_buffer.size());

  void *ref = CM_threads[proc].store_buffer.front().first;
  const SymData &blk = CM_threads[proc].store_buffer.front().second;

  if(!TB.atomic_store(blk)) { abort(); return; }

  if(DryRun) return;

  std::memcpy((uint8_t*)ref,(uint8_t*)blk.get_block(),blk.get_ref().size);

  for(unsigned i = 0; i < CM_threads[proc].store_buffer.size()-1; ++i){
    CM_threads[proc].store_buffer[i] = CM_threads[proc].store_buffer[i+1];
  }
  CM_threads[proc].store_buffer.pop_back();

  if(int(CM_threads[proc].store_buffer.size()) <= CM_threads[proc].partial_buffer_flush){
    assert(0 <= CM_threads[proc].partial_buffer_flush);
    /* The real thread was waiting for the buffer to flush. Enable the
     * real thread. */
    CM_threads[proc].partial_buffer_flush = -1;
    TB.mark_available(proc);
  }

  if(CM_threads[proc].store_buffer.empty()){
    /* Disable update thread. */
    TB.mark_unavailable(proc,0);
    /* If the real thread has terminated, then wake up other threads
     * which are waiting to join with this one. */
    if(Threads[proc].ECStack.empty()){
      for(int p : Threads[proc].AwaitingJoin){
        TB.mark_available(p);
      }
    }
  }
}

int CMInterpreter::newThread(const CPid &cpid){
  CM_threads.push_back(CMThread());
  return Interpreter::newThread(cpid);
}

bool CMInterpreter::isFence(llvm::Instruction &I){
  if(llvm::CallInst *CI = llvm::dyn_cast<llvm::CallInst>(&I)){
    llvm::Function *F = CI->getCalledFunction();
    if(F && F->isDeclaration() &&
       F->getIntrinsicID() == llvm::Intrinsic::not_intrinsic &&
       conf.extfun_no_fence.count(F->getName().str()) == 0){
      return true;
    }
    if(F && F->getName().str().find("__VERIFIER_atomic_") == 0) return true;
    {
      std::string asmstr;
      if(isInlineAsm(AnyCallInst(CI),&asmstr) && asmstr == "mfence") return true;
    }
  }else if(llvm::isa<llvm::StoreInst>(I)){
    return static_cast<llvm::StoreInst&>(I).getOrdering() == LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent;
  }else if(llvm::isa<llvm::FenceInst>(I)){
    return static_cast<llvm::FenceInst&>(I).getOrdering() == LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent;
  }else if(llvm::isa<llvm::AtomiCMmpXchgInst>(I)){
#ifdef LLVM_CMPXCHG_SEPARATE_SUCMESS_FAILURE_ORDERING
    llvm::AtomicOrdering suCM = static_cast<llvm::AtomiCMmpXchgInst&>(I).getSuCMessOrdering();
    llvm::AtomicOrdering fail = static_cast<llvm::AtomiCMmpXchgInst&>(I).getFailureOrdering();
    if(suCM != LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent || fail != LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent){
#else
    if(static_cast<llvm::AtomiCMmpXchgInst&>(I).getOrdering() != LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent){
#endif
      Debug::warn("CMInterpreter::isFence::cmpxchg") << "WARNING: Non-sequentially consistent CMPXCHG instruction interpreted as sequentially consistent.\n";
    }
    return true;
  }else if(llvm::isa<llvm::AtomicRMWInst>(I)){
    if(static_cast<llvm::AtomicRMWInst&>(I).getOrdering() != LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent){
      Debug::warn("CMInterpreter::isFence::rmw") << "WARNING: Non-sequentially consistent RMW instruction interpreted as sequentially consistent.\n";
    }
    return true;
  }
  return false;
}

void CMInterpreter::terminate(llvm::Type *RetTy, llvm::GenericValue Result){
  if(CurrentThread != 0){
    assert(RetTy == llvm::Type::getInt8PtrTy(RetTy->getContext()));
    Threads[CurrentThread].RetVal = Result;
  }
  if(CM_threads[CurrentThread].store_buffer.empty()){
    for(int p : Threads[CurrentThread].AwaitingJoin){
      TB.mark_available(p);
    }
  }
}

bool CMInterpreter::checkRefuse(llvm::Instruction &I){
  int tid;
  if(isPthreadJoin(I,&tid)){
    if(0 <= tid && tid < int(Threads.size()) && tid != CurrentThread){
      if(Threads[tid].ECStack.size() ||
         CM_threads[tid].store_buffer.size()){
        /* The awaited thread is still executing */
        TB.refuse_schedule();
        Threads[tid].AwaitingJoin.push_back(CurrentThread);
        return true;
      }
    }else{
      // Erroneous thread id
      // Allow execution (will produce an error trace)
    }
  }
  /* Refuse if I has fence semantics and the store buffer is
   * non-empty.
   */
  if(isFence(I) && !CM_threads[CurrentThread].store_buffer.empty()){
    CM_threads[CurrentThread].partial_buffer_flush = 0;
    TB.refuse_schedule();
    return true;
  }
  /* Refuse if I is a load and the latest entry in the store buffer
   * which overlaps with the memory location targeted by I does not
   * target precisely the same bytes as I.
   */
  if(llvm::isa<llvm::LoadInst>(I)){
    llvm::ExecutionContext &SF = ECStack()->back();
    llvm::GenericValue SRC = getOperandValue(static_cast<llvm::LoadInst&>(I).getPointerOperand(), SF);
    llvm::GenericValue *Ptr = (llvm::GenericValue*)GVTOP(SRC);
    Option<SymAddrSize> mr = TryGetSymAddrSize(Ptr,static_cast<llvm::LoadInst&>(I).getType());
    if (!mr) return false; /* Let it execute and segfault */
    for(int i = int(CM_threads[CurrentThread].store_buffer.size())-1; 0 <= i; --i){
      if(mr->overlaps(CM_threads[CurrentThread].store_buffer[i].second.get_ref())){
        if(*mr != CM_threads[CurrentThread].store_buffer[i].second.get_ref()){
          /* Block until this store buffer entry has disappeared from
           * the buffer.
           */
          CM_threads[CurrentThread].partial_buffer_flush =
            int(CM_threads[CurrentThread].store_buffer.size()) - i - 1;
          TB.refuse_schedule();
          return true;
        }
        break;
      }
    }
  }
  return Interpreter::checkRefuse(I);
}

void CMInterpreter::visitLoadInst(llvm::LoadInst &I){
  llvm::ExecutionContext &SF = ECStack()->back();
  llvm::GenericValue SRC = getOperandValue(I.getPointerOperand(), SF);
  llvm::GenericValue *Ptr = (llvm::GenericValue*)GVTOP(SRC);
  llvm::GenericValue Result;

  Option<SymAddrSize> Ptr_sas = GetSymAddrSize(Ptr,I.getType());
  if (!Ptr_sas) return;
  if(!TB.load(*Ptr_sas)) { abort(); return; }

  if(DryRun && DryRunMem.size()){
    assert(CM_threads[CurrentThread].store_buffer.empty());
    DryRunLoadValueFromMemory(Result, Ptr, *Ptr_sas, I.getType());
    SetValue(&I, Result, SF);
    return;
  }

  /* Check store buffer for ROWE opportunity. */
  for(int i = int(CM_threads[CurrentThread].store_buffer.size())-1; 0 <= i; --i){
    if(Ptr_sas->addr == CM_threads[CurrentThread].store_buffer[i].second.get_ref().addr){
      /* Read-Own-Write-Early */
      assert(Ptr_sas->size == CM_threads[CurrentThread].store_buffer[i].second.get_ref().size);
      LoadValueFromMemory(Result,(llvm::GenericValue*)CM_threads[CurrentThread].store_buffer[i].second.get_block(),I.getType());
      SetValue(&I, Result, SF);
      return;
    }
  }

  /* Load from memory */
  LoadValueFromMemory(Result, Ptr, I.getType());
  SetValue(&I, Result, SF);
}

void CMInterpreter::visitStoreInst(llvm::StoreInst &I){
  llvm::ExecutionContext &SF = ECStack()->back();
  llvm::GenericValue Val = getOperandValue(I.getOperand(0), SF);
  llvm::GenericValue *Ptr = (llvm::GenericValue *)GVTOP(getOperandValue(I.getPointerOperand(), SF));
  Option<SymData> sd = GetSymData(Ptr, I.getOperand(0)->getType(), Val);
  if (!sd) return;

  if(I.getOrdering() == LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent ||
     0 <= AtomicFunctionCall){
    /* Atomic store */
    assert(CM_threads[CurrentThread].store_buffer.empty());
    if(!TB.atomic_store(*sd)) { abort(); return; }
    if(DryRun){
      DryRunMem.push_back(std::move(*sd));
      return;
    }
    StoreValueToMemory(Val, Ptr, I.getOperand(0)->getType());
  }else{
    /* Store to buffer */
    if(!TB.store(*sd)) { abort(); return; }
    if(DryRun){
      DryRunMem.push_back(std::move(*sd));
      return;
    }
    CM_threads[CurrentThread].store_buffer.emplace_back(Ptr, std::move(*sd));
  }
}

void CMInterpreter::visitFenceInst(llvm::FenceInst &I){
  if(I.getOrdering() == LLVM_ATOMIC_ORDERING_SCOPE::SequentiallyConsistent){
    if(!TB.fence()) { abort(); return; }
  }
}

void CMInterpreter::visitAtomiCMmpXchgInst(llvm::AtomiCMmpXchgInst &I){
  assert(CM_threads[CurrentThread].store_buffer.empty());
  Interpreter::visitAtomiCMmpXchgInst(I);
}

void CMInterpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &I){
  assert(CM_threads[CurrentThread].store_buffer.empty());
  Interpreter::visitAtomicRMWInst(I);
}

void CMInterpreter::visitInlineAsm(llvm::CallInst &CS, const std::string &asmstr){
  if(asmstr == "mfence"){
    if(!TB.fence()) { abort(); return; }
  }else if(asmstr == ""){ // Do nothing
  }else{
    throw std::logic_error("Unsupported inline assembly: "+asmstr);
  }
}
