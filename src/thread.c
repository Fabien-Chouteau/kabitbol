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

#include "core.h"
#include "partition.h"
#include "thread.h"
#include "debug.h"
#include "mm/kalloc.h"
#include "scheduler.h"
#include "thread_state.h"
#include "asm.h"
#include "mm/access.h"

int partition_add_thread(struct partition *p, vaddr entry, uint32_t data)
{
	int status = 0;
	int i = 0;

	if (p->num_threads >= MAX_PARTITION_THREADS)
	{
		pdebug(DS_PARTITION, DL_ERROR, "Thread Limit reached: %d\r\n",
					 MAX_PARTITION_THREADS);
		return -ELIMIT;
	}

	i = p->num_threads++;

	p->threads[i].started = FALSE;
	p->threads[i].data    = data;
	p->threads[i].pc      = entry;
	p->threads[i].npc     = entry + 4;
	p->threads[i].part    = p;
	p->threads[i].thid    = i;
	p->threads[i].restore_counter = 1;

	p->threads[i].stack = vm_add_stack(p->as->stacks);
	if (p->threads[i].stack == NULL)
	{
		status = -ERROR;
		goto failnofree;
	}

	if ((status = vm_stack_enlarge(p->threads[i].stack)) != NOERROR)
		goto fail;

	p->threads[i].sp = p->threads[i].stack->start_sp;

	pdebug(DS_PARTITION, DL_INFO, "new thread sp at 0x%x\r\n", p->threads[i].sp);
	return i;

 fail:
	vm_rm_stack(p->as->stacks, p->threads[i].stack);
 failnofree:
	p->num_threads--;
	pdebug(DS_PARTITION, DL_ERROR, "partition_add_thread FAILED!\r\n");
	return status;
}

int thread_set_state(struct partition *p, uint32_t thid, uint32_t state)
{
	int ret = 0;

	if (thid > p->num_threads)
		return -EINVAL;

	switch (state)
	{
		case THREAD_STATE_RUN:
			ret = sched_run(&p->threads[thid]);
			break;

		default:
			return -EINVAL;
	}

	p->threads[thid].state = state;
	return ret;
}

/***************/
/* THREAD LIST */
/***************/

struct thread_list *thread_list_add(struct thread_list *l, struct thread *th)
{
	struct thread_list *ret = kalloc(sizeof (*ret));

	if (ret == NULL)
		return NULL;

	ret->th = th;
	ret->next = l;
	return ret;
}

struct thread_list *thread_list_rm(struct thread_list *l, struct thread *th)
{
	struct thread_list *prev = NULL;
	struct thread_list *parc = l;

	for (; parc != NULL && parc->th != th;
			 prev = parc, parc = parc->next)
		continue;

	if (parc == NULL)
		return l;

	if (parc == l)
		l = parc->next;
	else
		prev->next = parc->next;

	kfree(parc);
	return l;
}

