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
#include "mm/mm.h"
#include "mm/kalloc.h"
#include "mm/pt_alloc.h"
#include "mm/vm_manager.h"

ptd g_contexts_tab[MAX_CTX_NBR]
	__attribute__ ((aligned(MAX_CTX_NBR * sizeof (ptd))));

ptd g_kernel_lvl1[MM_LVL1_ENTRIES_NBR]
	__attribute__ ((aligned(MM_LVL1_ENTRIES_NBR * sizeof (ptd))));
ptd g_kernel_lvl2_heap[MM_LVL2_ENTRIES_NBR]
	__attribute__ ((aligned(MM_LVL2_ENTRIES_NBR * sizeof (ptd))));
ptd g_kernel_lvl3_heap[MM_LVL2_ENTRIES_NBR][MM_LVL3_ENTRIES_NBR]
	__attribute__ ((aligned(MM_LVL3_ENTRIES_NBR * sizeof (ptd))));

ptd g_kernel_lvl2_code[MM_LVL2_ENTRIES_NBR]
	__attribute__ ((aligned(MM_LVL2_ENTRIES_NBR * sizeof (ptd))));
ptd g_kernel_lvl3_code[MM_LVL2_ENTRIES_NBR][MM_LVL3_ENTRIES_NBR]
	__attribute__ ((aligned(MM_LVL3_ENTRIES_NBR * sizeof (ptd))));

int map_page(struct as *as, unsigned int vpage,
						 unsigned int ppage, unsigned int flags)
{
	unsigned int j  = 0;
	unsigned int i1 = mm_index1(vpage * PAGE_SIZE);
	unsigned int i2 = mm_index2(vpage * PAGE_SIZE);
	unsigned int i3 = mm_index3(vpage * PAGE_SIZE);
	ptd *p1 = NULL;
	ptd *p2 = NULL;
	ptd *p3 = NULL;


	if ((p1 = as->lvl1) == NULL)
		return -EINVAL;

	if ((p2 = MM_GET_PT_ADDR(p1[i1])) == NULL)
	{
		if ((p2 = pt_alloc(2)) == NULL)
			return -ENOMEM;

		for (j = 0; j < MM_LVL2_ENTRIES_NBR; j++)
			p2[j] = MM_ET_INVALID;

		p1[i1] = ((unsigned int)p2 >> 4) | MM_ET_PTD;
	}

	if ((p3 = MM_GET_PT_ADDR(p2[i2])) == NULL)
	{
		if ((p3 = pt_alloc(3)) == NULL)
			return -ENOMEM;

		for (j = 0; j < MM_LVL3_ENTRIES_NBR; j++)
			p3[j] = MM_ET_INVALID;

		p2[i2] = ((unsigned int)p3 >> 4) | MM_ET_PTD;
	}

	pdebug(DS_MM, DL_INFO, "ctx:[%x][%x][%x][%x] map_page 0x%x->0x%x\r\n", as->ctx_nbr, i1, i2, i3,
				 vpage * PAGE_SIZE, ppage * PAGE_SIZE);
	p3[i3] = (ppage * PAGE_SIZE >> 4) | MM_ET_PTE | flags;
	return NOERROR;
}

int unmap_page(struct as *as, unsigned int vpage)
{
	unsigned int i1 = mm_index1(vpage * PAGE_SIZE);
	unsigned int i2 = mm_index2(vpage * PAGE_SIZE);
	unsigned int i3 = mm_index3(vpage * PAGE_SIZE);
	ptd *p1 = NULL;
	ptd *p2 = NULL;
	ptd *p3 = NULL;

	if ((p1 = as->lvl1) == NULL)
		return -EINVAL;

	if ((p2 = MM_GET_PT_ADDR(p1[i1])) == NULL)
		return -EINVAL;

	if ((p3 = MM_GET_PT_ADDR(p2[i2])) == NULL)
		return -EINVAL;

	p3[i3] = MM_ET_INVALID;
	return NOERROR;
}

int kernel_map(unsigned int vpage, unsigned int ppage, unsigned int flags)
{
	unsigned int i1 = mm_index1(vpage * PAGE_SIZE);
	unsigned int i2 = mm_index2(vpage * PAGE_SIZE);
	unsigned int i3 = mm_index3(vpage * PAGE_SIZE);
	ptd *p2 = NULL;
	ptd *p3 = NULL;

	if (i1 != MM_KERNEL_LVL1_INDEX_CODE && i1 != MM_KERNEL_LVL1_INDEX_HEAP)
	{
		pdebug(DS_MM, DL_ERROR, "Kernel map outside LVL1 index\r\n");
		return -EINVAL;
	}
	/* all kernel pt are staticaly allocated */
	p2 = MM_GET_PT_ADDR(g_kernel_lvl1[i1]);
	p3 = MM_GET_PT_ADDR(p2[i2]);
	p3[i3] = ((ppage * PAGE_SIZE) >> 4) | MM_ET_PTE | flags;
	return NOERROR;
}

int kernel_unmap(unsigned int vpage)
{
	unsigned int i1 = mm_index1(vpage * PAGE_SIZE);
	unsigned int i2 = mm_index2(vpage * PAGE_SIZE);
	unsigned int i3 = mm_index3(vpage * PAGE_SIZE);
	ptd *p2 = NULL;
	ptd *p3 = NULL;

	if (i1 != MM_KERNEL_LVL1_INDEX_CODE && i1 != MM_KERNEL_LVL1_INDEX_HEAP)
	{
		pdebug(DS_MM, DL_ERROR, "Kernel unmap outside LVL1 index\r\n");
		return -EINVAL;
	}

	/* all kernel pt are staticaly allocated */
	p2 = MM_GET_PT_ADDR(g_kernel_lvl1[i1]);
	p3 = MM_GET_PT_ADDR(p2[i2]);
	p3[i3] = MM_ET_INVALID;
	return NOERROR;
}

int map_continuous(struct as *as, unsigned int vpage_start,
									 unsigned int ppage_start, unsigned int size,
									 unsigned int flags)
{
	unsigned int i = 0;

	for (i = 0; i < size; i++)
		if (as == &g_kas)
			kernel_map(vpage_start + i, ppage_start + i, flags);
		else
			map_page(as, vpage_start + i, ppage_start + i, flags);

	return NOERROR;
}

int unmap_continuous(struct as *as,
										 unsigned int vpage_start,
										 unsigned int size)
{
	unsigned int i = 0;
	for (i = 0; i < size; i++)
		if (as == &g_kas)
			kernel_unmap(vpage_start + i);
		else
			unmap_page(as, vpage_start + i);

	return NOERROR;
}

int mapping_init()
{
	unsigned int i = 0;
	unsigned int j = 0;

	/* init kernel mapping */
	for (i = 0; i < MM_LVL1_ENTRIES_NBR; i++)
		g_kernel_lvl1[i] = MM_ET_INVALID;

	g_kernel_lvl1[MM_KERNEL_LVL1_INDEX_CODE] = ((unsigned int)&g_kernel_lvl2_code >> 4)
		| MM_ET_PTD;
	g_kernel_lvl1[MM_KERNEL_LVL1_INDEX_HEAP] = ((unsigned int)&g_kernel_lvl2_heap >> 4)
		| MM_ET_PTD;

	for (i = 0; i < MM_LVL2_ENTRIES_NBR; i++)
	{
		g_kernel_lvl2_heap[i] = ((unsigned int)g_kernel_lvl3_heap[i] >> 4) | MM_ET_PTD;
		for (j = 0; j < MM_LVL3_ENTRIES_NBR; j++)
			g_kernel_lvl3_heap[i][j] = MM_ET_INVALID;

		g_kernel_lvl2_code[i] = ((unsigned int)g_kernel_lvl3_code[i] >> 4) | MM_ET_PTD;
		for (j = 0; j < MM_LVL3_ENTRIES_NBR; j++)
			g_kernel_lvl3_code[i][j] = MM_ET_INVALID;
	}

	/* map kernel */
	for (i = RAM_ADDR / PAGE_SIZE; i < ((unsigned int)g_brk / PAGE_SIZE) + 1; i++)
		kernel_map(i, i, MM_CACHEABLE | MM_ACC_S_RWE);

	pdebug(DS_MM, DL_INFO, " kernel init mapping 0x%x->0x%x\r\n", RAM_ADDR, (unsigned int)g_brk);

	for (i = 0; i < MAX_CTX_NBR; i++)
		g_contexts_tab[i] = MM_ET_INVALID;

	/* KERNEL CONTEXT */
	g_contexts_tab[0] = (unsigned int)g_kernel_lvl1 >> 4 | MM_ET_PTD;

	return NOERROR;
}

pte mapping_walk(as_s *as, vaddr ptr)
{
	unsigned int i1 = mm_index1(ptr);
	unsigned int i2 = mm_index2(ptr);
	unsigned int i3 = mm_index3(ptr);
	ptd *p1 = NULL;
	ptd *p2 = NULL;
	ptd *p3 = NULL;

	if ((p1 = as->lvl1) == NULL)
		return MM_ET_INVALID;

	if ((p2 = MM_GET_PT_ADDR(p1[i1])) == NULL)
		return MM_ET_INVALID;

	if ((p3 = MM_GET_PT_ADDR(p2[i2])) == NULL)
		return MM_ET_INVALID;

	return p3[i3] ;
}

