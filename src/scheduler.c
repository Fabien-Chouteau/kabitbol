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

#include "scheduler.h"
#include "timer.h"
#include "context.h"
#include "config.h"
#include "debug.h"
#include "thread.h"
#include "list.h"

struct thread *g_curr_th = NULL;
struct thread *g_next_th = NULL;


DECLARE_LIST(g_running);
DECLARE_LIST(g_sleeping);

int schedule(void)
{
	g_next_th = g_running.next->data;
	list_remove(&g_running, g_next_th);
	list_add_tail(&g_running, g_next_th);
	return NOERROR;
}

int sched_run(struct thread *t)
{
	pdebug(DS_SCHED, DL_DEBUG, "sched run : 0x%x\r\n", t);

	if (list_find(&g_running, t) != NULL)
		return NOERROR;

	if (list_find(&g_sleeping, t) != NULL)
		list_remove(&g_sleeping, t);

	list_add_head(&g_running, t);
	return NOERROR;
}

int sched_sleep(struct thread *t)
{

	if (list_find(&g_sleeping, t) != NULL)
		return NOERROR;

	if (list_find(&g_running, t) != NULL)
		list_remove(&g_running, t);

	list_add_head(&g_sleeping, t);
	return NOERROR;
}

struct thread *sched_elect_for_wake_up(list_s *l)
{
	return l->next->data;
}
