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

#ifndef VM_MANAGER_H_
# define VM_MANAGER_H_

# include "core.h"
# include "mm/vm_block.h"
# include "mm/vm_stacks.h"
# include "mm/mm.h"

typedef struct as
{
		unsigned int     ctx_nbr;
		ptd              *lvl1;
		uint32_t         brk;
		struct vm_block  *code;
		struct vm_block  *heap;
		struct vm_stacks *stacks;
} as_s;

struct as *new_as();
int     is_in_code_space(vaddr start, unsigned int size);
int32_t syscall_sbrk(struct as *as, uint32_t arg);

#endif	/* ! VM_MNAGER_H_ */
