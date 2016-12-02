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

#include "mutex.h"
#include "scheduler.h"

int mutex_lock(struct mutex *m, struct thread *th)
{
	if (m->lock == MUTEX_FREE)
	{
		m->lock = MUTEX_LOCKED;
		m->owner = th;
	}
	else
	{
		th->wait_for = m;
		sched_sleep(th);
		return list_add_tail(&m->waiting, th);
	}

	return NOERROR;
}

int mutex_unlock(struct mutex *m, struct thread *th)
{
	struct thread *elect = NULL;
	if (m->owner != th)
		return -EINVAL;

	if (list_is_empty(&m->waiting))
	{
		m->lock = MUTEX_FREE;
		return NOERROR;
	}

	elect = sched_elect_for_wake_up(&m->waiting);
	if (elect == NULL)
		return -EINVAL;

	list_remove(&m->waiting, elect);
	elect->wait_for = NULL;
	m->owner = elect;

	return NOERROR;
}
