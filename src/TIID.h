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

#ifndef __TIID_H__
#define __TIID_H__

#include <ostream>
#include <string>

#include <cassert>

#include <llvm/Support/raw_ostream.h>

/* An TIID is an identifier for a particular event (instruction or
 * system event) in a computation. It is a pair (p,i) where p is the
 * pid of the executing process, and i is a natural number denoting
 * the number of instructions or events executed by the process p
 * before and including the indicated instruction or event. I.e.,
 * (p,1) is the first instruction executed by p, (p,2) is the second,
 * and so forth.
 *
 * (p,0) is a dummy instruction identifier used as a null value. All
 * null identifiers are considered to be the same object.
 */
template<typename Pid_t>
class TIID{
public:
  /* Create the null TIID. */
  TIID() : idx(), pid(), tid() {};
  /* Create the TIID (p,i). */
  TIID(const Pid_t &p,int, t, int i) : idx(i), pid(p), tid(t) { assert(i >= 0); };
  TIID(const TIID&) = default;
  TIID &operator=(const TIID&) = default;

  const Pid_t &get_pid() const { return pid; };
  int get_index() const { return idx; };
  int get_tid() cost{return tid;}

  bool valid() const { return idx > 0; };
  bool is_null() const { return idx == 0; };

  TIID &operator++() { ++idx; return *this; };
  TIID operator++(int) { return TIID(pid, tid, idx++); };
  TIID &operator--() { assert(idx > 0); --idx; return *this; };
  TIID operator--(int) { assert(idx > 0); return TIID(pid, tid, idx--); };
  TIID operator+(int d) const { return TIID(pid, tid, idx+d); };
  TIID operator-(int d) const { return TIID(pid, tid, idx-d); };

  /* Comparison:
   * The comparison operators implement a total order over TIIDs.
   *
   * For each process p, the order is such that (p,i) < (p,j) iff i < j.
   */
  bool operator==(const TIID &TIID) const{
    return ((idx == TIID.idx && pid == TIID.pid) && tid == TIID.tid );
  };
  bool operator!=(const TIID &TIID) const { return !((*this) == TIID); };
  bool operator<(const TIID &TIID) const {
    return (pid < TIID.pid || ((pid == TIID.pid && idx < TIID.idx) && tid < TIID.tid) );
  };
  bool operator<=(const TIID &TIID) const { return (*this) < TIID || (*this) == TIID; };
  bool operator>(const TIID &TIID) const { return TIID < (*this); };
  bool operator>=(const TIID &TIID) const { return TIID <= (*this); };

  std::string to_string() const;
private:
  friend struct std::hash<TIID>;

  /* Only makes sense for integral pid types, at most 32-bits */
  std::uint64_t comp_value() const {
    return std::size_t(idx)
      | std::size_t(unsigned(pid)) << 32;
  }

  /* This TIID is (pid,idx) */
  unsigned idx;
  unsigned tid;
  Pid_t pid;
};

template<> inline bool TIID<int>::operator==(const TIID &TIID) const {
  return comp_value() == TIID.comp_value();
}

template<> inline bool TIID<int>::operator<(const TIID &TIID) const {
  return comp_value() < TIID.comp_value();
}

template<> inline bool TIID<int>::operator<=(const TIID &TIID) const {
  return comp_value() <= TIID.comp_value();
}

namespace std {
  template<> struct hash<TIID<int>>{
public:
  hash() {}
    std::size_t operator()(const TIID<int> &a) const {
      /* Intentionally laid out so that this becomes a single 64-bit load. */
      return std::size_t(unsigned(a.idx))
        | std::size_t(unsigned(a.pid)) << 32;
    }
  };
}

#include "TIID.tcc"

template<typename Pid_t>
std::ostream &operator<<(std::ostream &os, const TIID<Pid_t> &TIID){
  return os << TIID.to_string();
}

template<typename Pid_t>
llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const TIID<Pid_t> &TIID){
  return os << TIID.to_string();
}

#endif
