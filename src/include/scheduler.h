/*
 *  Copyright (C) 2009-2011 Fabien Chouteau
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SCHEDULER_H_
# define SCHEDULER_H_

#include "partition.h"
#include "thread.h"
#include "list.h"

extern struct thread *g_curr_th;
extern struct thread *g_next_th;

int schedule(void);

int sched_run(struct thread *t);
int sched_sleep(struct thread *t);

struct thread *sched_elect_for_wake_up(list_s *l);

#endif /* ! SCHEDULER_H_ */
