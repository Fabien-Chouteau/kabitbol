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

#include "mm/vm_manager.h"
#include "mm/pt_alloc.h"
#include "mm/kalloc.h"
#include "mm/mapping.h"
#include "mm/access.h"
#include "context.h"
#include "debug.h"
#include "scheduler.h"

struct as *new_as()
{
	static unsigned int ctx = 1;
	struct as *ret = NULL;
	int j = 0;

	ret = kalloc(sizeof (struct as));
	if (ret == NULL)
		return NULL;

	ret->stacks = kalloc(sizeof (struct vm_stacks));
	if (ret->stacks == NULL)
		return NULL;
	ret->stacks->as = ret;

	ret->code = kalloc(sizeof (struct vm_block));
	if (ret->code == NULL)
		return NULL;
	ret->code->flags = MM_ACC_RWE | MM_CACHEABLE;
	ret->code->as = ret;
	ret->code->start = 1; 				/* 1st page */
	ret->code->size_mapped		 = 0;
	ret->code->size_allocated = 0;
	ret->code->ilist = 0;

	ret->heap = kalloc(sizeof (struct vm_block));
	if (ret->heap == NULL)
		return NULL;
	ret->heap->flags = MM_ACC_RW | MM_CACHEABLE;
	ret->heap->as = ret;
	ret->heap->start = VM_HEAP_BASE_ADDR / PAGE_SIZE;
	ret->heap->size_mapped    = 0;
	ret->heap->size_allocated = 0;
	ret->heap->ilist = 0;
	ret->brk = VM_HEAP_BASE_ADDR;

	if (ctx >= MAX_CTX_NBR)
		return NULL;
	ret->ctx_nbr = ctx++;

	if ((ret->lvl1 = pt_alloc(1)) == NULL)
		return NULL;

	for (j = 0; j < MM_LVL1_ENTRIES_NBR; j++)
		ret->lvl1[j] = 0 | MM_ET_INVALID;

	/* always add the kernel lvl2 */
	ret->lvl1[MM_KERNEL_LVL1_INDEX_CODE] = ((unsigned int)g_kernel_lvl2_code >> 4) | MM_ET_PTD;
	ret->lvl1[MM_KERNEL_LVL1_INDEX_HEAP] = ((unsigned int)g_kernel_lvl2_heap >> 4) | MM_ET_PTD;
	g_contexts_tab[ret->ctx_nbr] = ((unsigned int)ret->lvl1 >> 4) | MM_ET_PTD;
	return ret;
}

int is_in_code_space(vaddr start, unsigned int size)
{
	return start >= PAGE_SIZE && (start + size) < RAM_ADDR;
}

int32_t syscall_sbrk(struct as *as, uint32_t arg)
{
	struct vm_block *heap = as->heap;
	int32_t  size   = (int32_t)arg;
	uint32_t newbrk = as->brk + size;
	int32_t  diff   = newbrk - (heap->size_mapped + heap->start) * PAGE_SIZE;

	if (size != 0)
	{
		if (diff > 0)
			vm_block_enlarge(as->heap, (diff / PAGE_SIZE) + 1);
		else
			vm_block_reduce(as->heap, -diff / PAGE_SIZE);

		as->brk = newbrk;
	}
	return (int32_t)(as->brk - size);
}
