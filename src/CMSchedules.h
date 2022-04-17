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
#ifndef __CM_SCEDULES_H__
#define __CM_SCEDULES_H__
#endif

#include <queue>
#include <memory>
#include "SymEv.h"
#include "vector"
#include "IID.h"
#include "VClock.h"

typedef int IPid;
typedef int Tid;

struct CMSchedule;

class CMSTransaction{
 public: 
  	CMSTransaction(IPid pid, Tid tid, unsigned tindex):pid(pid), tid(tid) , tindex(tindex) {}
  	
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
  
struct CMSEvent{
 public:
    CMSEvent(const IID<IPid> &iid, SymEv sym = {})
      : iid(iid), origin_iid(iid),sym(std::move(sym)) {}

    int alt;
    int size;
    bool pinned;
    
    IID<IPid> iid;
    IID<IPid> origin_iid;

    int read_from;
    
    Tid tid;
    
    std::vector<int> new_read_from;

    std::shared_ptr<CMSchedule> new_schedules;
    
    bool localread = false;
    
    bool swappable = true;

    bool current = false;

    SymEv sym;

    int depth;
};

struct CMSchedule{
  public:
    CMSchedule(int depth) {
      this->depth = depth;
    }
    int depth;
    std::vector<CMSEvent> events_before;
    std::vector<CMSTransaction> transactions_before;
    std::vector<int> new_read_from;
    std::vector<std::vector<CMSTransaction>> scheduled_transactions;
    std::vector<std::vector<CMSEvent>> scheduled_events;
    
  };

  class CMDepthCompare {
  public:
    bool operator()(const std::shared_ptr<CMSEvent> &a,
                  const std::shared_ptr<CMSEvent> &b) const {
      return a->depth < b->depth;
    }
  };
/*class DepthCompare {
public:
  bool operator()(const CMSEvent &a,
                const CMSEvent &b) const {
    return a.depth < b.depth;
  }
};*/

class CMSchedules {
 public:
 	std::priority_queue<std::shared_ptr<CMSEvent>, std::vector<std::shared_ptr<CMSEvent>>, CMDepthCompare> scheduler;
  //std::priority_queue<CMSEvent, std::vector<CMSEvent>, DepthCompare> scheduler;

  CMSEvent create_event();
};
