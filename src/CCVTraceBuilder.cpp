/* Copyright (C) 2021 Omkar Tuppe 
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
#include "CCVTraceBuilder.h"
#include "PrefixHeuristic.h"
#include "Timing.h"
#include "TraceUtil.h"
#include "Interpreter.h"
#include <sstream>
#include <stdexcept>

#define ANSIRed "\x1b[91m"
#define ANSIRst "\x1b[m"

#ifndef NDEBUG
#  define IFDEBUG(X) X
#else
#  define IFDEBUG(X)
#endif

static Timing::Context analysis_context("analysis");
static Timing::Context vclocks_context("vclocks");
static Timing::Context unfolding_context("unfolding");
static Timing::Context neighbours_context("neighbours");
static Timing::Context try_read_from_context("try_read_from");
static Timing::Context ponder_mutex_context("ponder_mutex");

CCVTraceBuilder::CCVTraceBuilder(const Configuration &conf)
    : TSOPSOTraceBuilder(conf) {
    threads.push_back(Thread(CPid(), -1));
    prefix_idx = -1;
    transaction_idx  = -1;
    replay = false;
    cancelled = false;
    last_full_memory_conflict = -1;
    last_md = 0;
    replay_point = 0;
    tasks_created = 0;
}

CCVTraceBuilder::~CCVTraceBuilder(){
}

bool CCVTraceBuilder::schedule(int *proc, int *aux, int *alt, bool *dryrun){
  if (cancelled) {
    assert(!std::any_of(threads.begin(), threads.end(), [](const Thread &t) {
                                                          return t.available;
                                                        }));
    return false;
  }
  *dryrun = false;
  *alt = 0;
  *aux = -1; /* No auxilliary threads in SC */
  if(replay){
    /* Are we done with the current Event? */
    if (0 <= prefix_idx && threads[curev().iid.get_pid()].last_event_index() <
        curev().iid.get_index() + curev().size - 1) {
      /* Continue executing the current Event */
      IPid pid = curev().iid.get_pid();
      *proc = pid;
      *alt = 0;
      if(!(threads[pid].available)) {
        llvm::dbgs() << "Trying to play process " << threads[pid].cpid << ", but it is blocked\n";
        llvm::dbgs() << "At replay step " << prefix_idx << ", iid "
                     << iid_string(IID<IPid>(pid, threads[curev().iid.get_pid()].last_event_index()))
                     << "\n";
        abort();
      }
      threads[pid].event_indices.push_back(prefix_idx);
      return true;
    } else if(prefix_idx + 1 == int(prefix.size())) {
      /* We are done replaying. Continue below... */
      assert(prefix_idx < 0 || (curev().sym.empty() ^ seen_effect)
             || (errors.size() && errors.back()->get_location()
                 == IID<CPid>(threads[curev().iid.get_pid()].cpid,
                              curev().iid.get_index())));
      replay = false;
    } else {
      /* Go to the next event. */
      assert(prefix_idx < 0 || (curev().sym.empty() ^ seen_effect)
             || (errors.size() && errors.back()->get_location()
                 == IID<CPid>(threads[curev().iid.get_pid()].cpid,
                              curev().iid.get_index())));
      seen_effect = false;
      ++prefix_idx;
      IPid pid = curev().iid.get_pid();
      *proc = pid;
      *alt = curev().alt;
      assert(threads[pid].available);
      threads[pid].event_indices.push_back(prefix_idx);
      return true;
    }
  }

  assert(!replay);
  /* Create a new Event */

  // TEMP: Until we have a SymEv for store()
  // assert(prefix_idx < 0 || !!curev().sym.size() == curev().may_conflict);

  /* Should we merge the last two events? */
  if(prefix.size() > 1 &&
     prefix[prefix.size()-1].iid.get_pid()
     == prefix[prefix.size()-2].iid.get_pid() &&
     !prefix[prefix.size()-1].may_conflict){
    assert(curev().sym.empty()); /* Would need to be copied */
    assert(curev().sym.empty()); /* Can't happen */
    prefix.pop_back();
    --prefix_idx;
    ++curev().size;
    assert(int(threads[curev().iid.get_pid()].event_indices.back()) == prefix_idx + 1);
    threads[curev().iid.get_pid()].event_indices.back() = prefix_idx;
  }


  /* Find an available thread. */
  for(unsigned p = 0; p < threads.size(); ++p){ // Loop through real threads
    if(threads[p].available &&
       (conf.max_search_depth < 0 || threads[p].last_event_index() < conf.max_search_depth)){
      /* Create a new Event */
      seen_effect = false;
      ++prefix_idx;
      assert(prefix_idx == int(prefix.size()));
      threads[p].event_indices.push_back(prefix_idx);
      prefix.emplace_back(IID<IPid>(IPid(p),threads[p].last_event_index()));
      *proc = p;
      return true;
    }
  }

  return false; // No available threads
}

void CCVTraceBuilder::refuse_schedule(){
  assert(prefix_idx == int(prefix.size())-1);
  assert(curev().size == 1);
  assert(!curev().may_conflict);
  IPid last_pid = curev().iid.get_pid();
  prefix.pop_back();
  assert(int(threads[last_pid].event_indices.back()) == prefix_idx);
  threads[last_pid].event_indices.pop_back();
  --prefix_idx;
  mark_unavailable(last_pid);
}

void CCVTraceBuilder::mark_available(int proc, int aux){
  threads[ipid(proc,aux)].available = true;
}

void CCVTraceBuilder::mark_unavailable(int proc, int aux){
  threads[ipid(proc,aux)].available = false;
}

bool CCVTraceBuilder::is_replaying() const {
  return prefix_idx < replay_point;
}

void CCVTraceBuilder::cancel_replay(){
  assert(replay == is_replaying());
  cancelled = true;
  replay = false;
}

void CCVTraceBuilder::metadata(const llvm::MDNode *md){
  if(curev().md == 0){
    curev().md = md;
  }
  last_md = md;
}

bool CCVTraceBuilder::sleepset_is_empty() const{
  return true;
}

Trace *CCVTraceBuilder::get_trace() const{
  std::vector<IID<CPid> > cmp;
  SrcLocVectorBuilder cmp_md;
  std::vector<Error*> errs;
  for(unsigned i = 0; i < prefix.size(); ++i){
    cmp.push_back(IID<CPid>(threads[prefix[i].iid.get_pid()].cpid,prefix[i].iid.get_index()));
    cmp_md.push_from(prefix[i].md);
  };
  for(unsigned i = 0; i < errors.size(); ++i){
    errs.push_back(errors[i]->clone());
  }
  Trace *t = new IIDSeqTrace(cmp,cmp_md.build(),errs);
  t->set_blocked(!sleepset_is_empty());
  return t;
}

bool CCVTraceBuilder::reset(){

	for(unsigned i=0 ; i < transactions.size() ; ++i){
			transactions[i].global_variables.clear();
      transactions[i].happens_after.clear();
      transactions[i].read_from.clear();
      transactions[i].modification_order.clear();
	}
  transactions.clear();
  transaction_idx = -1;
  
  prefix.clear();
  CPS = CPidSystem();
  threads.clear();
  threads.push_back(Thread(CPid(),-1));
  mutexes.clear();
  cond_vars.clear();
  mem.clear();
  mutex_deadlocks.clear();
  last_full_memory_conflict = -1;
  prefix_idx = -1;
  replay = false;
  cancelled = false;
  last_md = 0;
  tasks_created = 0;
  reset_cond_branch_log();

  return true;
}

IID<CPid> CCVTraceBuilder::get_iid() const{
  IPid pid = curev().iid.get_pid();
  int idx = curev().iid.get_index();
  return IID<CPid>(threads[pid].cpid,idx);
}

static std::string rpad(std::string s, int n){
  while(int(s.size()) < n) s += " ";
  return s;
}

static std::string lpad(const std::string &s, int n){
  return std::string(std::max(0, n - int(s.size())), ' ') + s;
}

std::string CCVTraceBuilder::iid_string(std::size_t pos) const{
  return iid_string(prefix[pos]);
}

std::string CCVTraceBuilder::iid_string(const Event &event) const{
  IPid pid = event.iid.get_pid();
  int index = event.iid.get_index();
  std::stringstream ss;
  ss << "(" << threads[pid].cpid << "," << index;
  if(event.size > 1){
    ss << "-" << index + event.size - 1;
  }
  ss << ")";
  if(event.alt != 0){
    ss << "-alt:" << event.alt;
  }
  return ss.str();
}

std::string CCVTraceBuilder::iid_string(IID<IPid> iid) const{
  return iid_string(find_process_event(iid.get_pid(), iid.get_index()));
}

void CCVTraceBuilder::debug_print() const {
  llvm::dbgs() << "CCVTraceBuilder (debug print, replay until " << replay_point << "):\n";
  
}

bool CCVTraceBuilder::spawn(){
  IPid parent_ipid = curev().iid.get_pid();
  CPid child_cpid = CPS.spawn(threads[parent_ipid].cpid);
  threads.push_back(Thread(child_cpid,prefix_idx));
  curev().may_conflict = true;
  return record_symbolic(SymEv::Spawn(threads.size() - 1));
}

bool CCVTraceBuilder::store(const SymData &sd){
  assert(false && "Cannot happen");
  abort();
  return true;
}

bool CCVTraceBuilder::atomic_store(const SymData &sd){
  if (!record_symbolic(SymEv::Store(sd))) return false;
  if(!transactions.empty() && curev().iid.get_pid() == transactions.back().pid) 
  	curev().tid = transactions.back().pid;
  do_atomic_store(sd);
  return true;
}

void CCVTraceBuilder::do_atomic_store(const SymData &sd){
  const SymAddrSize &ml = sd.get_ref();
  curev().may_conflict = true;

  /* See previous updates reads to ml */
  for(SymAddr b : ml){
    ByteInfo &bi = mem[b];

    /* Register in memory */
    bi.last_update = prefix_idx;
  }
}

bool CCVTraceBuilder::atomic_rmw(const SymData &sd){
  if (!record_symbolic(SymEv::Rmw(sd))) return false;
  do_load(sd.get_ref());
  do_atomic_store(sd);
  return true;
}

bool CCVTraceBuilder::load(const SymAddrSize &ml){
  if (!record_symbolic(SymEv::Load(ml))) return false;
  if(!transactions.empty() && curev().iid.get_pid() == transactions.back().pid) 
  	curev().tid = transactions.back().pid;
  do_load(ml);
  return true;
}

void CCVTraceBuilder::do_load(const SymAddrSize &ml){
  curev().may_conflict = true;
  int lu = mem[ml.addr].last_update;
  curev().read_from = lu;

  assert(lu == -1 || get_addr(lu) == ml);
  assert(std::all_of(ml.begin(), ml.end(), [lu,this](SymAddr b) {
             return mem[b].last_update == lu;
           }));
}

bool CCVTraceBuilder::compare_exchange
(const SymData &sd, const SymData::block_type expected, bool success){
  if(success){
    if (!record_symbolic(SymEv::CmpXhg(sd, expected))) return false;
    do_load(sd.get_ref());
    do_atomic_store(sd);
  }else{
    if (!record_symbolic(SymEv::CmpXhgFail(sd, expected))) return false;
    do_load(sd.get_ref());
  }
  return true;
}

bool CCVTraceBuilder::full_memory_conflict(){
  invalid_input_error("CCV does not support black-box functions with memory effects");
  return false;
  if (!record_symbolic(SymEv::Fullmem())) return false;
  curev().may_conflict = true;

  // /* See all pervious memory accesses */
  // for(auto it = mem.begin(); it != mem.end(); ++it){
  //   do_load(it->second);
  // }
  // last_full_memory_conflict = prefix_idx;

  // /* No later access can have a conflict with any earlier access */
  // mem.clear();
  return true;
}

bool CCVTraceBuilder::fence(){
  return true;
}

bool CCVTraceBuilder::join(int tgt_proc){
  if (!record_symbolic(SymEv::Join(tgt_proc))) return false;
  curev().may_conflict = true;
  add_happens_after_thread(prefix_idx, tgt_proc);
  return true;
}

bool CCVTraceBuilder::mutex_lock(const SymAddrSize &ml){
  if (!record_symbolic(SymEv::MLock(ml))) return false;

  Mutex &mutex = mutexes[ml.addr];
  curev().may_conflict = true;
  curev().read_from = mutex.last_access;

  mutex.last_lock = mutex.last_access = prefix_idx;
  mutex.locked = true;
  return true;
}

bool CCVTraceBuilder::mutex_lock_fail(const SymAddrSize &ml){
  assert(!conf.mutex_require_init || mutexes.count(ml.addr));
  IFDEBUG(Mutex &mutex = mutexes[ml.addr];)
  assert(0 <= mutex.last_lock && mutex.locked);

  IPid current = curev().iid.get_pid();
  auto &deadlocks = mutex_deadlocks[ml.addr];
  assert(!std::any_of(deadlocks.cbegin(), deadlocks.cend(),
                      [current](IPid blocked) {
                        return blocked == current;
                      }));
  deadlocks.push_back(current);
  return true;
}

bool CCVTraceBuilder::mutex_trylock(const SymAddrSize &ml){
  Mutex &mutex = mutexes[ml.addr];
  if (!record_symbolic(mutex.locked ? SymEv::MTryLockFail(ml) : SymEv::MTryLock(ml)))
    return false;
  curev().read_from = mutex.last_access;
  curev().may_conflict = true;

  mutex.last_access = prefix_idx;
  if(!mutex.locked){ // Mutex is free
    mutex.last_lock = prefix_idx;
    mutex.locked = true;
  }
  return true;
}

bool CCVTraceBuilder::mutex_unlock(const SymAddrSize &ml){
  if (!record_symbolic(SymEv::MUnlock(ml))) return false;
  Mutex &mutex = mutexes[ml.addr];
  curev().read_from = mutex.last_access;
  curev().may_conflict = true;
  assert(0 <= mutex.last_access);

  mutex.last_access = prefix_idx;
  mutex.locked = false;

  /* No one is blocking anymore! Yay! */
  mutex_deadlocks.erase(ml.addr);
  return true;
}

bool CCVTraceBuilder::mutex_init(const SymAddrSize &ml){
  if (!record_symbolic(SymEv::MInit(ml))) return false;
  assert(mutexes.count(ml.addr) == 0);
  curev().read_from = -1;
  curev().may_conflict = true;
  mutexes[ml.addr] = Mutex(prefix_idx);
  return true;
}

bool CCVTraceBuilder::mutex_destroy(const SymAddrSize &ml){
  if (!record_symbolic(SymEv::MDelete(ml))) return false;
  Mutex &mutex = mutexes[ml.addr];
  curev().read_from = mutex.last_access;
  curev().may_conflict = true;

  mutex.last_access = prefix_idx;
  mutex.locked = false;
  return true;
}

bool CCVTraceBuilder::cond_init(const SymAddrSize &ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  if (!record_symbolic(SymEv::CInit(ml))) return false;
  if(cond_vars.count(ml.addr)){
    pthreads_error("Condition variable initiated twice.");
    return false;
  }
  curev().may_conflict = true;
  cond_vars[ml.addr] = CondVar(prefix_idx);
  return true;
}

bool CCVTraceBuilder::cond_signal(const SymAddrSize &ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  if (!record_symbolic(SymEv::CSignal(ml))) return false;
  curev().may_conflict = true;

  auto it = cond_vars.find(ml.addr);
  if(it == cond_vars.end()){
    pthreads_error("cond_signal called with uninitialized condition variable.");
    return false;
  }
  CondVar &cond_var = it->second;
  VecSet<int> seen_events = {last_full_memory_conflict};
  if(cond_var.waiters.size() > 1){
    if (!register_alternatives(cond_var.waiters.size())) return false;
  }
  assert(0 <= curev().alt);
  assert(cond_var.waiters.empty() || curev().alt < int(cond_var.waiters.size()));
  if(cond_var.waiters.size()){
    /* Wake up the alt:th waiter. */
    int i = cond_var.waiters[curev().alt];
    assert(0 <= i && i < prefix_idx);
    IPid ipid = prefix[i].iid.get_pid();
    assert(!threads[ipid].available);
    threads[ipid].available = true;
    seen_events.insert(i);

    /* Remove waiter from cond_var.waiters */
    for(int j = curev().alt; j < int(cond_var.waiters.size())-1; ++j){
      cond_var.waiters[j] = cond_var.waiters[j+1];
    }
    cond_var.waiters.pop_back();
  }
  cond_var.last_signal = prefix_idx;

  return true;
}

bool CCVTraceBuilder::cond_broadcast(const SymAddrSize &ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  if (!record_symbolic(SymEv::CBrdcst(ml))) return false;
  curev().may_conflict = true;

  auto it = cond_vars.find(ml.addr);
  if(it == cond_vars.end()){
    pthreads_error("cond_broadcast called with uninitialized condition variable.");
    return false;
  }
  CondVar &cond_var = it->second;
  for(int i : cond_var.waiters){
    assert(0 <= i && i < prefix_idx);
    IPid ipid = prefix[i].iid.get_pid();
    assert(!threads[ipid].available);
    threads[ipid].available = true;
    // seen_events.insert(i);
  }
  cond_var.waiters.clear();
  cond_var.last_signal = prefix_idx;

  return true;
}

bool CCVTraceBuilder::cond_wait(const SymAddrSize &cond_ml, const SymAddrSize &mutex_ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  {
    auto it = mutexes.find(mutex_ml.addr);
    if(it == mutexes.end()){
      if(conf.mutex_require_init){
        pthreads_error("cond_wait called with uninitialized mutex object.");
      }else{
        pthreads_error("cond_wait called with unlocked mutex object.");
      }
      return false;
    }
    Mutex &mtx = it->second;
    if(mtx.last_lock < 0 || prefix[mtx.last_lock].iid.get_pid() != curev().iid.get_pid()){
      pthreads_error("cond_wait called with mutex which is not locked by the same thread.");
      return false;
    }
  }

  if (!mutex_unlock(mutex_ml)) return false;
  if (!record_symbolic(SymEv::CWait(cond_ml))) return false;
  curev().may_conflict = true;

  IPid pid = curev().iid.get_pid();

  auto it = cond_vars.find(cond_ml.addr);
  if(it == cond_vars.end()){
    pthreads_error("cond_wait called with uninitialized condition variable.");
    return false;
  }
  it->second.waiters.push_back(prefix_idx);
  threads[pid].available = false;

  return true;
}

bool CCVTraceBuilder::cond_awake(const SymAddrSize &cond_ml, const SymAddrSize &mutex_ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  assert(cond_vars.count(cond_ml.addr));
  CondVar &cond_var = cond_vars[cond_ml.addr];
  add_happens_after(prefix_idx, cond_var.last_signal);

  if (!mutex_lock(mutex_ml)) return false;
  if (!record_symbolic(SymEv::CAwake(cond_ml))) return false;
  curev().may_conflict = true;

  return true;
}

int CCVTraceBuilder::cond_destroy(const SymAddrSize &ml){
  invalid_input_error("CCV does not support condition variables");
  return false;
  const int err = (EBUSY == 1) ? 2 : 1; // Chose an error value different from EBUSY
  if (!record_symbolic(SymEv::CDelete(ml))) return err;

  curev().may_conflict = true;

  auto it = cond_vars.find(ml.addr);
  if(it == cond_vars.end()){
    pthreads_error("cond_destroy called on uninitialized condition variable.");
    return err;
  }
  CondVar &cond_var = it->second;

  int rv = cond_var.waiters.size() ? EBUSY : 0;
  cond_vars.erase(ml.addr);
  return rv;
}

bool CCVTraceBuilder::register_alternatives(int alt_count){
  invalid_input_error("CCV does not support nondeterministic events");
  return false;
  curev().may_conflict = true;
  if (!record_symbolic(SymEv::Nondet(alt_count))) return false;
  // if(curev().alt == 0) {
  //   for(int i = curev().alt+1; i < alt_count; ++i){
  //     curev().races.push_back(Race::Nondet(prefix_idx, i));
  //   }
  // }
  return true;
}

template <class Iter>
static void rev_recompute_data
(SymData &data, VecSet<SymAddr> &needed, Iter end, Iter begin){
  for (auto pi = end; !needed.empty() && (pi != begin);){
    const SymEv &p = *(--pi);
    switch(p.kind){
    case SymEv::STORE:
    case SymEv::UNOBS_STORE:
    case SymEv::RMW:
    case SymEv::CMPXHG:
      if (data.get_ref().overlaps(p.addr())) {
        for (SymAddr a : p.addr()) {
          if (needed.erase(a)) {
            data[a] = p.data()[a];
          }
        }
      }
      break;
    default:
      break;
    }
  }
}

void CCVTraceBuilder::add_happens_after(unsigned second, unsigned first){
  assert(first != ~0u);
  assert(second != ~0u);
  assert(first != second);
  assert(first < second);
  assert((long long)second <= prefix_idx);

  std::vector<unsigned> &vec = prefix[second].happens_after;
  if (vec.size() && vec.back() == first) return;

  vec.push_back(first);
}

void CCVTraceBuilder::add_happens_after_thread(unsigned second, IPid thread){
  assert((int)second == prefix_idx);
  if (threads[thread].event_indices.empty()) return;
  add_happens_after(second, threads[thread].event_indices.back());
}

/* Filter the sequence first..last from all elements that are less than
 * any other item. The sequence is modified in place and an iterator to
 * the position beyond the last included element is returned.
 *
 * Assumes less is transitive and asymetric (a strict partial order)
 */
template< class It, class LessFn >
static It frontier_filter(It first, It last, LessFn less){
  It fill = first;
  for (It current = first; current != last; ++current){
    bool include = true;
    for (It check = first; include && check != fill;){
      if (less(*current, *check)){
        include = false;
        break;
      }
      if (less(*check, *current)){
        /* Drop check from fill set */
        --fill;
        std::swap(*check, *fill);
      }else{
        ++check;
      }
    }
    if (include){
      /* Add current to fill set */
      if (fill != current) std::swap(*fill, *current);
      ++fill;
    }
  }
  return fill;
}

int CCVTraceBuilder::compute_above_clock(unsigned i) {
  int last = -1;
  IPid ipid = transactions[i].get_pid();
  int tidx = transactions[i].get_index();
  if (tidx > 1) {
    ///last = find_process_event(ipid, tidx-1);
    last = find_process_transaction(ipid, tidx-1);
    transactions[i].clock = transactions[last].clock;
    //transactions[i].above_clock = transactions[last].above_clock;
  } else {
    transactions[i].clock = VClock<IPid>();
    //transactions[i].above_clock = transactions[i].clock;
    const Thread &t = threads[ipid];
    if (t.spawn_event >= 0)
      add_transaction_happens_after(i, t.spawn_event);
  }
  transactions[i].clock[ipid] = tidx;
  //transactions[i].above_clock[ipid] = tidx;

  /* First add the non-reversible edges */
  for (unsigned j : transactions[i].happens_after){
    assert(j < i);
    transactions[i].clock += transactions[j].clock;
    //transactions[i].above_clock +=transactions[j].above_clock;
  }

  transactions[i].above_clock = transactions[i].clock;
  return last;
}

void CCVTraceBuilder::compute_vclocks(){
  Timing::Guard timing_guard(vclocks_context);
  /* The first event of a thread happens after the spawn event that
   * created it.
   */
  std::vector<llvm::SmallVector<unsigned,2>> happens_after(transactions.size());
  for (unsigned i = 0; i < transactions.size(); i++){
    /* First add the non-reversible edges */
    int last = compute_above_clock(i);

    if (last != -1) happens_after[last].push_back(i);
    for (unsigned j : transactions[i].happens_after){
      happens_after[j].push_back(i);
    }


    
    /* Then add read-from */
    if(!transactions[i].read_from.empty()){
      for(unsigned j = 0; j < transactions[i].read_from.size() ; ++j){
        if (transactions[i].read_from[j] != -1) {
          transactions[i].clock += transactions[transactions[i].read_from[j]].clock;
          //transactions[i].above_clock += transactions[transactions[i].read_from[j]].above_clock;
          //happens_after[transactions[i].read_from[j]].push_back(i);
        }
      }
    }// [po U rf ]* 

    //transactions[i].above_clock = transactions[i].clock;   

    if(!transactions[i].modification_order.empty()){
      for(unsigned j = 0; j < transactions[i].modification_order.size() ; ++j){
        if (transactions[i].modification_order[j] != -1) {
          //transactions[i].clock += transactions[transactions[i].modification_order[j]].clock;
          //happens_after[transactions[i].modification_order[j]].push_back(i);
        }
      }
    }
  } // [po U rf U co]* 


  /* The top of our vector clock lattice, initial value of the below
   * clock (excluding itself) for evey event */
  VClock<IPid> top;
  for (unsigned i = 0; i < threads.size(); ++i)
    top[i] = threads[i].transaction_indices.size();
  below_clocks.assign(threads.size(), transactions.size(), top);

  for (unsigned i = transactions.size();;) {
    if (i == 0) break;
    auto clock = below_clocks[--i];
    Transaction &t = transactions[i];
    clock[t.get_pid()] = t.get_index();
    for (unsigned j : happens_after[i]) {
      assert (i < j);
      clock -= below_clocks[j];
    }
  }
}


bool CCVTraceBuilder::record_symbolic(SymEv event){
  if (!replay) {
    assert(!seen_effect);
    /* New event */
    curev().sym = std::move(event);
    seen_effect = true;
  } else {
    /* Replay. SymEv::set() asserts that this is the same event as last time. */
    SymEv &last = curev().sym;
    assert(!seen_effect);
    if (!last.is_compatible_with(event)) {
      auto pid_str = [this](IPid p) { return threads[p].cpid.to_string(); };
      nondeterminism_error("Event with effect " + last.to_string(pid_str)
                           + " became " + event.to_string(pid_str)
                           + " when replayed");
      return false;
    }
    last = event;
    seen_effect = true;
  }
  return true;
}


bool CCVTraceBuilder::is_load(unsigned i) const {
  const SymEv &e = prefix[i].sym;
  return e.kind == SymEv::LOAD || e.kind == SymEv::RMW
    || e.kind == SymEv::CMPXHG || e.kind == SymEv::CMPXHGFAIL;
}

bool CCVTraceBuilder::is_minit(unsigned i) const {
  return prefix[i].sym.kind == SymEv::M_INIT;
}

bool CCVTraceBuilder::is_mdelete(unsigned i) const {
  return prefix[i].sym.kind == SymEv::M_DELETE;
}


bool CCVTraceBuilder::is_store(unsigned i) const {
  const SymEv &e = prefix[i].sym;
  return e.kind == SymEv::STORE || e.kind == SymEv::CMPXHG
    || e.kind == SymEv::RMW;
}


bool CCVTraceBuilder::is_store_when_reading_from(unsigned i, int read_from) const {
  const SymEv &e = prefix[i].sym;
  if (e.kind == SymEv::STORE || e.kind == SymEv::RMW)
    return true;
  if (e.kind != SymEv::CMPXHG && e.kind != SymEv::CMPXHGFAIL) return false;
  SymData expected = e.expected();
  SymData actual = get_data(read_from, e.addr());
  assert(e.addr() == actual.get_ref());
  return memcmp(expected.get_block(), actual.get_block(), e.addr().size) == 0;
}

SymAddrSize CCVTraceBuilder::get_addr(unsigned i) const {
  const SymEv &e = prefix[i].sym;
  if (e.has_addr()) {
    return e.addr();
  }
  abort();
}

SymData CCVTraceBuilder::get_data(int i, const SymAddrSize &addr) const {
  if (i == -1) {
    SymData ret(addr, addr.size);
    memset(ret.get_block(), 0, addr.size);
    return ret;
  }
  const SymEv &e = prefix[i].sym;
  assert(e.has_data());
  assert(e.addr() == addr);
  return e.data();
}


bool CCVTraceBuilder::happens_before(const Event &e,
                                     const VClock<int> &c) const {
  return c.includes(e.iid);
}

bool CCVTraceBuilder::can_rf_by_vclocks
(int r, int ow, int w) const {
  /* Is the write after the read? */
  if (w != -1 && happens_before(prefix[r], prefix[w].clock)) abort();

  /* Is the original write always before the read, and the new write
   * before the original?
   */
  if (ow != -1 && (w == -1 || happens_before(prefix[w], prefix[ow].clock))) {
    if (happens_before(prefix[ow], prefix[r].above_clock)) return false;
  }

  return true;
}

bool CCVTraceBuilder::can_swap_by_vclocks(int r, int w) const {
  if (happens_before(prefix[r], prefix[w].above_clock)) return false;
  return true;
}

bool CCVTraceBuilder::can_swap_lock_by_vclocks(int f, int u, int s) const {
  if (happens_before(prefix[f], prefix[s].above_clock)) return false;
  return true;
}


std::vector<int> CCVTraceBuilder::iid_map_at(int event) const{
  std::vector<int> map(threads.size(), 1);
  for (int i = 0; i < event; ++i) {
    iid_map_step(map, prefix[i]);
  }
  return map;
}

void CCVTraceBuilder::iid_map_step(std::vector<int> &iid_map, const Event &event) const{
  if (iid_map.size() <= unsigned(event.iid.get_pid())) iid_map.resize(event.iid.get_pid()+1, 1);
  iid_map[event.iid.get_pid()] += event.size;
}

void CCVTraceBuilder::iid_map_step_rev(std::vector<int> &iid_map, const Event &event) const{
  iid_map[event.iid.get_pid()] -= event.size;
}

inline Option<unsigned> CCVTraceBuilder::
try_find_process_event(IPid pid, int index) const{
  assert(pid >= 0 && pid < int(threads.size()));
  assert(index >= 1);
  if (index > int(threads[pid].event_indices.size())){
    return nullptr;
  }

  unsigned k = threads[pid].event_indices[index-1];
  assert(k < prefix.size());
  assert(prefix[k].size > 0);
  assert(prefix[k].iid.get_pid() == pid
         && prefix[k].iid.get_index() <= index
         && (prefix[k].iid.get_index() + prefix[k].size) > index);

  return k;
}

inline unsigned CCVTraceBuilder::find_process_event(IPid pid, int index) const{
  assert(pid >= 0 && pid < int(threads.size()));
  assert(index >= 1 && index <= int(threads[pid].event_indices.size()));
  unsigned k = threads[pid].event_indices[index-1];
  assert(k < prefix.size());
  assert(prefix[k].size > 0);
  assert(prefix[k].iid.get_pid() == pid
         && prefix[k].iid.get_index() <= index
         && (prefix[k].iid.get_index() + prefix[k].size) > index);

  return k;
}



long double CCVTraceBuilder::estimate_trace_count() const{
  return estimate_trace_count(0);
}

bool CCVTraceBuilder::check_for_cycles() {
  return false;
}

long double CCVTraceBuilder::estimate_trace_count(int idx) const{
  if(idx > int(prefix.size())) return 0;
  if(idx == int(prefix.size())) return 1;

  long double count = 42;
  for(int i = int(prefix.size())-1; idx <= i; --i){
    count += prefix[i].sleep_branch_trace_count;
    // count += std::max(0, int(prefix.children_after(i)))
    //   * (count / (1 + prefix[i].sleep.size()));
  }

  return count;
}

//TODO
inline unsigned CCVTraceBuilder::find_process_transaction(IPid pid, int index) const{
  assert(pid >= 0 && pid < int(threads.size()));
  assert(index >= 1 && index <= int(threads[pid].transaction_indices.size()));
  unsigned k = threads[pid].transaction_indices[index-1];
  assert(k < transactions.size());
  assert(transactions[k].get_pid() == pid
         && transactions[k].get_index() < index);

  return k;
}
void CCVTraceBuilder::add_transaction_happens_after(Tid second, Tid first){
  assert(first != ~0u);
  assert(second != ~0u);
  assert(first != second);
  assert(first < second);
  assert((long long)second <= transaction_idx);

  std::vector<Tid> &vec = transactions[second].happens_after;
  if (vec.size() && vec.back() == first) return;

  vec.push_back(first);
}

void CCVTraceBuilder::add_transaction_happens_after_thread(Tid second, IPid thread){
  assert((int)second == transaction_idx);
  if (threads[thread].transaction_indices.empty()) return;
  add_happens_after(second, threads[thread].transaction_indices.back());
}

bool CCVTraceBuilder::transaction_happens_before(const Transaction &t,
                                     const VClock<int> &clock) const {
  IID<IPid> tiid(t.get_pid(),t.get_index());
  return clock.includes(tiid);
}

bool CCVTraceBuilder::has_store_on_var(void *ptr, unsigned i) const {
  const Transaction &t = transactions[i];
  if(t.global_variables.count(ptr)){
    return true;
  }
  return false;
}


void CCVTraceBuilder::createNextEvent(){
      auto p = curev().iid.get_pid();
 			++prefix_idx;
      assert(prefix_idx == int(prefix.size()));
      threads[p].event_indices.push_back(prefix_idx);
      prefix.emplace_back(IID<IPid>(IPid(p),threads[p].last_event_index()));
  
  }
bool CCVTraceBuilder::is_begin(unsigned i) const {
  const SymEv &e = prefix[i].sym;
  return e.kind == SymEv::BEGIN;
}

bool CCVTraceBuilder::is_end(unsigned i) const {
  const SymEv &e = prefix[i].sym;
  return e.kind == SymEv::END;
}

void CCVTraceBuilder::beginTransaction(int tid) {
  assert(tid > 0);
	IPid pid = curev().iid.get_pid();
  bool r = record_symbolic(SymEv::Begin(tid));
	++transaction_idx;
  assert(transaction_idx == int(transactions.size()));
  threads[pid].transaction_indices.push_back(transaction_idx);
  Transaction t(pid,tid,threads[pid].last_transaction_index());
  transactions.emplace_back(t);
  if(!transactions.empty() && curev().iid.get_pid() == transactions.back().pid) 
  	curev().tid = transactions.back().tid;
}

void CCVTraceBuilder::endTransaction(int tid) {
  assert(tid > 0);
  if(!transactions.empty() && curev().iid.get_pid() == transactions.back().pid){
  	curev().tid = transactions.back().tid;
    bool r = record_symbolic(SymEv::End(transactions.back().tid));
    //temp += transactions.back().tid;
 }
}

int CCVTraceBuilder:: performWrite(void *ptr, llvm::GenericValue val){
	IPid pid = curev().iid.get_pid();
	Transaction &t = transactions.back();
	if(!transactions.empty() && curev().iid.get_pid() == t.pid){
		if(t.global_variables.count(ptr)){
			t.global_variables[ptr] = val;
		}
		else{
			t.global_variables.insert({ptr,val});
		}
		return transaction_idx;
	}
	else
		return 0;
}

int CCVTraceBuilder::performRead(void *ptr ,llvm::Type *typ){
	int tid = curev().tid;

  compute_vclocks(); // computes [po U rf]* and [po U rf U co]*

  std::map<const void *,std::vector<Tid>> writes_by_address;
  std::vector<std::unordered_map<const void *,std::vector<Tid>>>
    writes_by_process_and_address(threads.size());
  for (unsigned j = 0; j < transactions.size(); ++j) {

    if (has_store_on_var(ptr,j))     
      writes_by_address[ptr].push_back(j);

    if (has_store_on_var(ptr,j))
      writes_by_process_and_address[transactions[j].get_pid()][ptr].push_back(j);
  }

  for (unsigned p = 0; p < threads.size(); ++p){
    const std::vector<Tid> &writes
          = writes_by_process_and_address[p][ptr];
    for(unsigned j=0; j < writes.size(); ++j){
        //if(!transaction_happens_before(transactions[writes[j]],transactions[tid].clock)){
        if(writes[j] <= tid){
            curev().can_read_from.push_back(writes[j]);
        }
    }
  }

  if(!curev().can_read_from.empty()) {
    //curev().can_read_from[0];
    temp = curev().can_read_from.size();
    return 1;
  }
  else{
    //return 0;
    Transaction &t = transactions.back();
    if(!transactions.empty() && curev().iid.get_pid() == t.pid){
      if(t.global_variables.count(ptr)){
        return transaction_idx;
      }
    }
    else{
        return 0;
    }
  }
  /*
	Transaction &t = transactions.back();
	if(!transactions.empty() && curev().iid.get_pid() == t.pid){
		if(t.global_variables.count(ptr)){
			return transaction_idx;
		}
	}
	else{
		return 0;
	}*/
}

uint64_t CCVTraceBuilder::tracecount(){
	uint64_t t = 0;
	int64_t value = 0;
	/*for(unsigned i=0 ; i < prefix.size() ; ++i){
		if(is_store(i) && prefix[i].tid > 0)
			t++;
	}*/
	for(unsigned i=0 ; i < transactions.size() ; ++i){
			for(auto j = transactions[i].global_variables.begin() ; j != transactions[i].global_variables.end() ; j++)
				//value = j->second.IntVal.getSExtValue();
				//t = t + value;
				t++;
	}
	t = temp;
	return t;
}