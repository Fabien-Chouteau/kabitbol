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
#include "debug.h"
#include "mm/kalloc.h"
#include "mm/vm_manager.h"
#include "mm/mapping.h"

extern char __brk;
char *g_brk = NULL;

struct as g_kas = {
	.heap = NULL,
};					/* fake as to handle kernel heap */

static int kalloc_virt = 0;

int kalloc_init()
{
	struct vm_block *b = NULL;

	kalloc_virt = 0;
	g_brk = &__brk;
	pdebug(DS_MM, DL_INFO, "Kalloc initialized brk=0x%x\r\n", g_brk);

	b = kalloc(sizeof (*b));
	if (b == NULL)
		return -ENOMEM;

	b->flags = MM_ACC_S_RWE | MM_CACHEABLE;
	b->as    = &g_kas;
	b->start = (RAM_ADDR + MM_LVL1_PAGE_SIZE) / PAGE_SIZE;
	b->size_mapped    = 0;
	b->size_allocated = 0;
	b->as    = &g_kas;
	b->ilist = NULL;
	g_kas.lvl1 = g_kernel_lvl1;
	g_kas.heap = b;
	return NOERROR;
}

void *sbrk(int size)
{
	g_brk += size;

	if (kalloc_virt)
	{
		unsigned int last_addr =
			(g_kas.heap->start + g_kas.heap->size_mapped) * PAGE_SIZE;
		long add_size = (unsigned int)g_brk - last_addr;

 		if (abs(add_size) >= PAGE_SIZE)
		{
			if (add_size > 0)
				vm_block_enlarge(g_kas.heap, (add_size / PAGE_SIZE) + 1);
			/* else */
			/* 	vm_block_reduce(g_kas.heap, (-add_size / PAGE_SIZE)); */
		}
	}

	return g_brk - size;
}

void *kalloc(unsigned int size)
{
	size += 4 - (size % 4);
	return (void *)sbrk(size);
}

void kfree(void *chunk)
{
	(void) chunk;
}

void kalloc_switch()
{
 	/* pre-alloc one page */
	vm_block_enlarge(g_kas.heap, 1);
	kalloc_virt = 1;
	g_brk = (char *)(g_kas.heap->start * PAGE_SIZE);
	pdebug(DS_MM, DL_INFO, "Kalloc re-initialized brk=0x%x\r\n", g_brk);
}
