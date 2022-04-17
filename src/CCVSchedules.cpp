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
#include "CCVSchedules.h"
#include "PrefixHeuristic.h"
#include "Interpreter.h"
#include <stdexcept>
#include <set>
#include <iterator>

#define ANSIRed "\x1b[91m"
#define ANSIRst "\x1b[m"

#ifndef NDEBUG
#  define IFDEBUG(X) X
#else
#  define IFDEBUG(X)
#endif

SEvent CCVSchedules::create_event(IID<Ipid> iid, int depth, std::shared_ptr<Schedule> schedule ){
    SEvent event(iid);
    event.depth = depth;
    event.new_schedules = schedule;
}