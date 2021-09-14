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

#include "Trace.h"
#include "TraceUtil.h"

#include <set>
#include <sstream>

Trace::Trace(const std::vector<Error*> &errors, bool blk)
  : errors(errors), blocked(blk) {
}

Trace::~Trace(){
  for(unsigned i = 0; i < errors.size(); ++i){
    delete errors[i];
  }
}

IIDSeqTrace::IIDSeqTrace(const std::vector<IID<CPid> > &cmp,
                         SrcLocVector cmpmd,
                         const std::vector<Error*> &errors,
                         TIDSeqTrace trns,
                         bool blk)
  : Trace(errors,blk), computation(cmp), computation_md(std::move(cmpmd)), trnsTrace(trns) {
}

IIDSeqTrace::IIDSeqTrace(const std::vector<IID<CPid> > &cmp,
                         SrcLocVector cmpmd,
                         const std::vector<Error*> &errors,
                         bool blk)
  : Trace(errors,blk), computation(cmp), computation_md(std::move(cmpmd)) {
}
IIDSeqTrace::~IIDSeqTrace(){
}

std::string Trace::to_string(int _ind){
  if(errors.size()){
    std::string s = "Errors found:\n";
    for(Error *e : errors){
      s += e->to_string()+"\n";
    }
    return s;
  }else{
    return "No errors found.\n";
  }
}

std::string IIDSeqTrace::to_string(int _ind) {
  std::string s;
  std::string ind;
  int transaction_idx = -1;
  assert(_ind >= 0);
  while(_ind){
    ind += " ";
    --_ind;
  }

  /* Setup individual indentation per process */
  std::map<CPid,std::string> cpind;
  {
    std::set<CPid> cpids;
    for(unsigned i = 0; i < computation.size(); ++i){
      cpids.insert(computation[i].get_pid());
    }

    std::string s = "";
    for(auto it = cpids.begin(); it != cpids.end(); ++it){
      cpind[*it] = s;
      s += ind;
    }
  }

  /* Identify error locations */
  std::map<int,Error*> error_locs;
  {
    for(unsigned i = 0; i < errors.size(); ++i){
      //error_locs[errors[i]->get_location()] = errors[i];
      int loc = -1;
      for(unsigned j = 0; j < computation.size(); ++j){
        if(computation[j].get_pid() == errors[i]->get_location().get_pid()){
          if(errors[i]->get_location().get_index() < computation[j].get_index()) break;
          loc = j;
        }
      }
      assert(0 <= loc);
      error_locs[loc] = errors[i];
    }
  }

  for(unsigned i = 0; i < computation.size(); ++i){
    std::string iid_str = ind + cpind[computation[i].get_pid()] + computation[i].to_string();
    //s += iid_str;
    if(computation[i].get_pid().is_auxiliary()){
      s+=" UPDATE";
    }
    std::string strns;
    {
      if(trnsTrace.computation_sym[i].type == 1){
          std::stringstream ss;
          std::stringstream ss1;
          transaction_idx++;

          int tpid = trnsTrace.transactions[transaction_idx].pid;
          trnsTrace.del[tpid].insert({trnsTrace.transactions[transaction_idx].tid , true});
          std::vector<int> delivery;

          //TODO: Update the Delivery code
          if(!trnsTrace.transactions[transaction_idx].read_from.empty()){
            std::stringstream ss;
            ss << " Reads From [";
            int j = 0;
            const std::vector<int> &reads = trnsTrace.transactions[transaction_idx].read_from;
            for(; j < reads.size() - 1 ; ++j) {
              if(reads[j] == -1)
                ss << " " << "init" << " ,";
              else
                ss << " " << trnsTrace.transactions[reads[j]].tid << " ,";
              if(reads[j]!= -1 && !trnsTrace.del[tpid].count(trnsTrace.transactions[reads[j]].tid)){
                trnsTrace.del[tpid].insert({trnsTrace.transactions[reads[j]].tid, false});
                delivery.push_back(trnsTrace.transactions[reads[j]].tid);
              }

            }
            if(reads[j] == -1)
                ss << " " << "init";
            else
                ss << " " << trnsTrace.transactions[reads[j]].tid;
            ss << " ]";
            if(reads[j] != -1 && !trnsTrace.del[tpid].count(trnsTrace.transactions[reads[j]].tid)){
                trnsTrace.del[tpid].insert({trnsTrace.transactions[reads[j]].tid, false});
                delivery.push_back(trnsTrace.transactions[reads[j]].tid);
            }
            s += iid_str;
            s += ss.str();
            s += "\n";

            if(!delivery.empty()){
              for(auto del : delivery){
                std::stringstream ss;
                ss << " DEL(" << tpid << "," << del << ")";
                s += iid_str;
                s += ss.str();
                s += "\n";
              }
            }
          }
          if(!trnsTrace.transactions[transaction_idx].modification_order.empty()){
            std::stringstream ss;
            ss << " Modification Order [";
            int j = 0;
            const std::vector<unsigned> &modification_orders = trnsTrace.transactions[transaction_idx].modification_order;
            for(; j < modification_orders.size() - 1 ; ++j){
              ss << " " << trnsTrace.transactions[modification_orders[j]].tid << " ,";
            }
            ss << " " << trnsTrace.transactions[modification_orders[j]].tid;
            ss << " ]";
            s += iid_str;
            s += ss.str();
            s += "\n";
          }
          ss1 << " Begin_Transaction (" << trnsTrace.transactions[transaction_idx].pid << " , ";
          ss1 << trnsTrace.transactions[transaction_idx].tid << ") ";
          s += iid_str;
          s += ss1.str();
          s += "\n";
          ss << " Transactid ID : " << trnsTrace.transactions[transaction_idx].tid;
          ss << "\t Vector Clock[hb]: " << trnsTrace.transactions[transaction_idx].above_clock.to_string();
          ss << "\t Vector Clock[hb U mo]: " << trnsTrace.transactions[transaction_idx].clock.to_string();
          s += iid_str;
          s += ss.str();
        }
      else if(auto md = computation_md[i]){
        std::stringstream ss;
        ss << " " << TraceUtil::basename(md.file) << ":" << md.line;
        strns = TraceUtil::get_src_line_verbatim(md);
        if(strns.find("}") == 0 && transaction_idx >= 0) {
          ss << ": " << "End_Transaction (" << trnsTrace.transactions[transaction_idx].pid << " , ";
          ss << trnsTrace.transactions[transaction_idx].tid << ") ";
        }
        else  
          ss << ": " << TraceUtil::get_src_line_verbatim(md);
        s += iid_str;
        s += ss.str();
      }
      s += "\n";
    }
    if(error_locs.count(i)){
      // Indentation
      {
        int sz = iid_str.size();
        while(sz--){
          s += " ";
        }
      }
      // Error
      std::string errstr = error_locs[i]->to_string();
      if(errstr.find("\n") == std::string::npos){
        s += " Error: " + errstr;
      }else{
        s += " Error";
      }
      s += "\n";
    }
  }
  return s;
}

Error *AssertionError::clone() const{
  return new AssertionError(loc,condition);
}

std::string AssertionError::to_string() const{
  return "Assertion violation at "+loc.to_string()+": ("+condition+")";
}

Error *PthreadsError::clone() const{
  return new PthreadsError(loc,msg);
}

std::string PthreadsError::to_string() const{
  return "Pthreads error at "+loc.to_string()+": "+msg;
}

Error *SegmentationFaultError::clone() const{
  return new SegmentationFaultError(loc);
}

std::string SegmentationFaultError::to_string() const{
  return "Segmentation fault at "+loc.to_string();
}

Error *RobustnessError::clone() const{
  return new RobustnessError(loc);
}

std::string RobustnessError::to_string() const{
  return "The trace contains a happens-before cycle.";
}

Error *MemoryError::clone() const{
  return new MemoryError(loc,msg);
}

std::string MemoryError::to_string() const{
  return "Memory error at "+loc.to_string()+": ("+msg+")";
}

Error *NondeterminismError::clone() const{
  return new NondeterminismError(loc,msg);
}

std::string NondeterminismError::to_string() const{
  return "Nondeterminism detected at "+loc.to_string()+": "+msg;
}

Error *InvalidInputError::clone() const{
  return new InvalidInputError(loc,msg);
}

std::string InvalidInputError::to_string() const{
  return "Operation at "+loc.to_string()+" not supported: "+msg;
}

auto SrcLocVector::operator[](std::size_t index) const -> LocRef {
  const SrcLoc &loc = locations[index];
  return {loc.line, string_table[loc.file], string_table[loc.dir]};
}
