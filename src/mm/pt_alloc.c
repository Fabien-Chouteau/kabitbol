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
#include "mm/pt_alloc.h"
#include "mm/ppages.h"
#include "mm/mm.h"
#include "mm/kalloc.h"
#include "mm/mapping.h"

struct pt_allocator g_pt_alloc;

ptd *pt_alloc(unsigned int lvl)
{
	struct pt_slab *sl = NULL;
	struct ppage   *pp = NULL;
	unsigned int tsize = 0;
	unsigned int tnbr  = 0;
	unsigned int i     = 0;

	if (lvl == 1)
	{
		sl    = g_pt_alloc.lvl_1;
		tsize = MM_LVL1_ENTRIES_NBR * sizeof (ptd);
		tnbr  = PAGE_SIZE / tsize;
	}
	else if (lvl == 2 || lvl == 3)
	{
		sl    = g_pt_alloc.lvl_2_3;
		tsize = MM_LVL2_ENTRIES_NBR * sizeof (ptd);
		tnbr  = PAGE_SIZE / tsize;
	}
	else
		return NULL;

	while (sl != NULL)
	{
		if (sl->free_cnt != 0)
		{
			for (i = 1; sl->map & (unsigned int)(0x1 << i); i++)
				continue;

			sl->map = sl->map | (unsigned int)(0x1 << i);
			sl->free_cnt--;
			return (ptd *)(sl->pp->start * PAGE_SIZE + tsize * i);
		}
	}

	if ((pp = ppage_alloc(1)) == NULL)
		return NULL;

	if ((sl = kalloc(sizeof (*sl))) == NULL)
	{
		ppage_free(pp);
		return NULL;
	}


	kernel_map(pp->start, pp->start, MM_ACC_S_RWE);
	sl->free_cnt = tnbr - 1;
	sl->pp = pp;
	sl->map = 1;

	if (lvl == 1)
	{
		sl->next = g_pt_alloc.lvl_1;
		g_pt_alloc.lvl_1 = sl;
	}
	else
	{
		sl->next = g_pt_alloc.lvl_2_3;
		g_pt_alloc.lvl_2_3 = sl;
	}

	return (ptd *)(sl->pp->start * PAGE_SIZE);
}

int pt_alloc_init()
{
	g_pt_alloc.lvl_1 = NULL;
	g_pt_alloc.lvl_2_3 = NULL;
	return NOERROR;
}
