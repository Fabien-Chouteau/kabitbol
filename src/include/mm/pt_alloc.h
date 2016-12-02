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

#ifndef PT_ALLOC_H_
# define PT_ALLOC_H_

# include "mm/mm.h"
# include "mm/ppages.h"

struct pt_slab;
struct pt_slab
{
		struct ppage   *pp;
		unsigned char  free_cnt;
		unsigned int   map;
		struct pt_slab *next;
};

struct pt_allocator
{
		struct pt_slab *lvl_1;
		struct pt_slab *lvl_2_3;
};

ptd *pt_alloc(unsigned int lvl);
int pt_alloc_init();

#endif	/* ! PT_ALLOC_H_ */
