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

#ifndef THREAD_H_
# define THREAD_H_

#include "core.h"
#include "mm/vm_stacks.h"

struct mutex;

struct thread
{
		vaddr        pc;
		vaddr        npc;
		vaddr        sp;
		uint32_t     restore_counter;
		uint32_t     state;
		uint32_t     thid;

		uint32_t     data;
		uint8_t      started;

		struct stack *stack;
		struct partition *part;

		/* LOCK */
		struct mutex *wait_for;
};

struct thread_list;
struct thread_list
{
		struct thread      *th;
		struct thread_list *next;
};


int partition_add_thread(struct partition *p, vaddr entry, uint32_t data);

int thread_set_state(struct partition *p, uint32_t thid, uint32_t state);

struct thread_list *thread_list_add(struct thread_list *l, struct thread *th);
struct thread_list *thread_list_rm(struct thread_list *l, struct thread *th);

#endif	/* ! THREAD_H_ */
