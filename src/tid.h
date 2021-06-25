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

#ifndef __TID_H__
#define __TID_H__

#include <ostream>
#include <string>

#include <cassert>

#include <llvm/Support/raw_ostream.h>

/* An TID is an identifier for a transaction
 */
template<typename Pid_t>
class TID{
public:
  /* Create the null TID. */
  TID() : tid(), pid() {};
  /* Create the TID (p,i). */
  TID(const Pid_t &p, int i) : tid(i), pid(p) { assert(i >= 0); };
  TID(const TID&) = default;
  TID &operator=(const TID&) = default;

  const Pid_t &get_pid() const { return pid; };
  int get_index() const { return tid; };

  bool valid() const { return tid > 0; };
  bool is_null() const { return tid == 0; };

  TID &operator++() { ++tid; return *this; };
  TID operator++(int) { return TID(pid,tid++); };
  TID &operator--() { assert(tid > 0); --tid; return *this; };
  TID operator--(int) { assert(tid > 0); return TID(pid,tid--); };
  TID operator+(int d) const { return TID(pid,tid+d); };
  TID operator-(int d) const { return TID(pid,tid-d); };

  /* Comparison:
   * The comparison operators implement a total order over TIDs.
   *
   * For each process p, the order is such that (p,i) < (p,j) iff i < j.
   */
  bool operator==(const TID &TID) const{
    return (tid == TID.tid && pid == TID.pid);
  };
  bool operator!=(const TID &TID) const { return !((*this) == TID); };
  bool operator<(const TID &TID) const {
    return (pid < TID.pid || (pid == TID.pid && tid < TID.tid));
  };
  bool operator<=(const TID &TID) const { return (*this) < TID || (*this) == TID; };
  bool operator>(const TID &TID) const { return TID < (*this); };
  bool operator>=(const TID &TID) const { return TID <= (*this); };

  std::string to_string() const;
private:
  friend struct std::hash<TID>;

  /* Only makes sense for integral pid types, at most 32-bits */
  std::uint64_t comp_value() const {
    return std::size_t(tid)
      | std::size_t(unsigned(pid)) << 32;
  }

  /* This TID is (pid,tid) */
  unsigned tid;
  Pid_t pid;
};

template<> inline bool TID<int>::operator==(const TID &TID) const {
  return comp_value() == TID.comp_value();
}

template<> inline bool TID<int>::operator<(const TID &TID) const {
  return comp_value() < TID.comp_value();
}

template<> inline bool TID<int>::operator<=(const TID &TID) const {
  return comp_value() <= TID.comp_value();
}

namespace std {
  template<> struct hash<TID<int>>{
public:
  hash() {}
    std::size_t operator()(const TID<int> &a) const {
      /* Intentionally laid out so that this becomes a single 64-bit load. */
      return std::size_t(unsigned(a.tid))
        | std::size_t(unsigned(a.pid)) << 32;
    }
  };
}

#include "TID.tcc"

template<typename Pid_t>
std::ostream &operator<<(std::ostream &os, const TID<Pid_t> &TID){
  return os << TID.to_string();
}

template<typename Pid_t>
llvm::raw_ostream &operator<<(llvm::raw_ostream &os, const TID<Pid_t> &TID){
  return os << TID.to_string();
}

#endif
