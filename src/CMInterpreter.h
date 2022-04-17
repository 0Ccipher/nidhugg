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

#include <config.h>
#ifndef __CM_INTERPRETER_H__
#define __CM_INTERPRETER_H__

#include "Interpreter.h"
#include "CMTraceBuilder.h"

/* A CMInterpreter is an interpreter running under the CM
 * semantics. The execution should be guided by scheduling from a
 * CMTraceBuilder.
 */
class CMInterpreter : public llvm::Interpreter{
public:
  explicit CMInterpreter(llvm::Module *M, CMTraceBuilder &TB,
                          const Configuration &conf = Configuration::default_conf);
  virtual ~CMInterpreter();

  static std::unique_ptr<CMInterpreter>
  create(llvm::Module *M, CMTraceBuilder &TB,
         const Configuration &conf = Configuration::default_conf,
         std::string *ErrorStr = 0);

  virtual void visitLoadInst(llvm::LoadInst &I);
  virtual void visitStoreInst(llvm::StoreInst &I);
  virtual void visitFenceInst(llvm::FenceInst &I);
  virtual void visitAtomiCMmpXchgInst(llvm::AtomiCMmpXchgInst &I);
  virtual void visitAtomicRMWInst(llvm::AtomicRMWInst &I);
  virtual void visitInlineAsm(llvm::CallInst &CI, const std::string &asmstr);
protected:
  virtual void runAux(int proc, int aux);
  virtual int newThread(const CPid &cpid);
  virtual bool isFence(llvm::Instruction &I);
  virtual bool checkRefuse(llvm::Instruction &I);
  virtual void terminate(llvm::Type *RetTy, llvm::GenericValue Result);

  /* Additional information for a thread, supplementing the
   * information in Interpreter::Thread.
   */
  class CMThread{
  public:
    CMThread() : partial_buffer_flush(-1){};
    /* The CM store buffer of this thread. Newer entries are further
     * to the back.
     */
    std::vector<std::pair<void*,SymData>> store_buffer;
    /* When partial_buffer_flush >= 0, it signals that this thread is
     * blocked, waiting for its store buffer update to memory. The
     * thread will continue to be blocked until the size of its store
     * buffer equals partial_buffer_flush.
     *
     * partial_buffer_flush == 0 indicates that the thread is blocking
     * until its store buffer is empty. This is the case e.g. when the
     * next instruction of the thread is a fence.
     */
    int partial_buffer_flush;
  };
  /* All threads that are or have been running during this execution
   * have an entry in Threads, in the order in which they were
   * created.
   */
  std::vector<CMThread> CM_threads;
};

#endif

