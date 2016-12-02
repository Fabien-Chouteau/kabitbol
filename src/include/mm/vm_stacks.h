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

#ifndef VM_STACKS_H_
# define VM_STACKS_H_

# include "mm/mm.h"
# include "config.h"

# define VM_STACKS_FLAGS (MM_ACC_RW | MM_CACHEABLE)

struct as;

/*
 * Alloc stack one ppage at a time;
 */
struct stack;
struct stack
{
		vaddr        start_sp;
		unsigned int start_page;
		unsigned int size;
		struct as    *as;
		struct ppage *list;
		struct stack *next;
};

struct vm_stacks
{
		unsigned int num;
		struct as    *as;
		struct stack *list;
};

int vm_stack_enlarge(struct stack *s);
int vm_stack_reduce(struct stack *s);
struct stack *vm_add_stack(struct vm_stacks *vms);
int vm_rm_stack(struct vm_stacks *vms, struct stack *s);

#endif	/* ! VM_STACKS_H_ */
