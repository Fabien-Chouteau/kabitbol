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

#ifndef MAPPING_H_
# define MAPPING_H_
# include "mm/vm_manager.h"

extern ptd g_kernel_lvl1[MM_LVL1_ENTRIES_NBR];
extern ptd g_kernel_lvl2_code[MM_LVL2_ENTRIES_NBR];
extern ptd g_kernel_lvl2_heap[MM_LVL2_ENTRIES_NBR];

int map_page(struct as *as, unsigned int vpage,
						 unsigned int ppage, unsigned int flags);

int unmap_page(struct as *as, unsigned int vpage);

int map_continuous(struct as *as, unsigned int vpage_start,
									 unsigned int ppage_start, unsigned int size,
									 unsigned int flags);

int unmap_continuous(struct as *as,
										 unsigned int vpage_start,
										 unsigned int size);

int kernel_map(unsigned int vpage, unsigned int ppage, unsigned int flags);

int mapping_init();

pte mapping_walk(as_s *as, vaddr ptr);

#endif	/* ! MAPPING_H_ */
