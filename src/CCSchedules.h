/* Copyright (C) 2021 Omkar Tuppe
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
#ifndef __CC_SCEDULES_H__
#define __CC_SCEDULES_H__
#endif

#include <queue>
#include <memory>
#include "SymEv.h"
#include "vector"
#include "IID.h"
#include "VClock.h"

typedef int IPid;
typedef int Tid;

struct CCSchedule;

class CCSTransaction{
 public: 
  	CCSTransaction(IPid pid, Tid tid, unsigned tindex):pid(pid), tid(tid) , tindex(tindex) {}
  	
  	IPid pid;
  	Tid tid;
  	unsigned tindex;

    VClock<IPid> clock, above_clock;

    IPid get_pid() const{
      return pid;
    }

    Tid get_tid() const{
      return tid;
    }

    unsigned get_index() const{
      return tindex;
    }
};
  
struct CCSEvent{
 public:
    CCSEvent(const IID<IPid> &iid, SymEv sym = {})
      : iid(iid), origin_iid(iid),sym(std::move(sym)) {}

    int alt;
    int size;
    bool pinned;
    
    IID<IPid> iid;
    IID<IPid> origin_iid;

    int read_from;
    
    Tid tid;
    
    std::vector<int> new_read_from;

    std::shared_ptr<CCSchedule> new_schedules;
    
    bool localread = false;
    
    bool swappable = true;

    bool current = false;

    SymEv sym;

    int depth;
};

struct CCSchedule{
  public:
    CCSchedule(int depth) {
      this->depth = depth;
    }
    int depth;
    std::vector<CCSEvent> events_before;
    std::vector<CCSTransaction> transactions_before;
    std::vector<int> new_read_from;
    std::vector<std::vector<CCSTransaction>> scheduled_transactions;
    std::vector<std::vector<CCSEvent>> scheduled_events;
    
  };

  class CCDepthCompare {
  public:
    bool operator()(const std::shared_ptr<CCSEvent> &a,
                  const std::shared_ptr<CCSEvent> &b) const {
      return a->depth < b->depth;
    }
  };
/*class DepthCompare {
public:
  bool operator()(const CCSEvent &a,
                const CCSEvent &b) const {
    return a.depth < b.depth;
  }
};*/

class CCSchedules {
 public:
 	std::priority_queue<std::shared_ptr<CCSEvent>, std::vector<std::shared_ptr<CCSEvent>>, CCDepthCompare> scheduler;
  //std::priority_queue<CCSEvent, std::vector<CCSEvent>, DepthCompare> scheduler;

  CCSEvent create_event();
};
