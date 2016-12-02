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

#ifndef MUTEX_H_
# define MUTEX_H_

# include "list.h"
# include "types.h"
# include "thread.h"

# define MUTEX_FREE   0
# define MUTEX_LOCKED 1

struct mutex
{
		list_s        waiting;
		struct thread *owner;
		uint8_t       lock;
};

#endif	/* ! MUTEX_H_ */
