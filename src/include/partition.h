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

#ifndef PARTITION_H_
# define PARTITION_H_

# include "core.h"
# include "config.h"
# include "mm/vm_manager.h"
# include "thread.h"

struct partition
{
		unsigned int  context_nbr;
		struct as     *as;

		unsigned int  num_threads;
		struct thread threads[MAX_PARTITION_THREADS];
};

extern struct partition g_partitions[PARTITION_NBR];

extern char _binary_initpartition_o_start[];
extern char _binary_initpartition_o_end[];

int partition_init(void);

#endif /* ! PARTITION_H_ */
