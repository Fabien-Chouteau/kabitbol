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
#include "mm/vm_block.h"
#include "mm/ppages.h"
#include "mm/mapping.h"

/**
 * Add @size to block
 * @size in page
 */
int vm_block_enlarge(struct vm_block *b, unsigned int size)
{
	unsigned int unmapped = 0;
	unsigned int mapsize = 0;
	struct ppage *pp = NULL;

	printf("vm_block_enlarge(%u)\r\n", size);
	vm_block_info(b);

	if (size == 0)
		return NOERROR;

	if (b->size_allocated > b->size_mapped)
	{
		/*
		 * we have more ppage allocated, so use them.
		 * the unallocated page are allways in the first ppage of the list.
		 */
		unmapped = b->size_allocated - b->size_mapped;
		mapsize = min_t(unsigned int, size, unmapped);

		map_continuous(b->as, b->start + b->size_mapped,
									 b->ilist->end - unmapped + 1, mapsize,
									 b->flags);
		b->size_mapped += mapsize;
		size -= mapsize;
	}

	if (size == 0)
		return 0; /* we have reached the requested size */

	/* we need to allocate ppage */
	pp = ppage_alloc(size);
	if (pp == NULL)
		return -ENOMEM;

	b->size_allocated += pp->end - pp->start + 1;/* add the real size allocated */
	pp->next = b->ilist;
	b->ilist = pp;
	map_continuous(b->as, b->start + b->size_mapped, pp->start, size, b->flags);
	b->size_mapped += size;

	return NOERROR;
}

/**
 * @size in page
 */
int vm_block_reduce(struct vm_block *b, unsigned int size)
{
	unsigned int unmapped = 0;
	unsigned int mapped   = 0;
	unsigned int pagesize = 0;
	struct ppage *pp      = NULL;

	printf("vm_block_reduce(%u)\r\n", size);

	if (size == 0)
		return NOERROR;

	while (size > 0)
	{
		unmapped = b->size_allocated - b->size_mapped;
		pagesize = b->ilist->end - b->ilist->start + 1;
		mapped   = pagesize - unmapped;

		if (mapped <= size)
		{
			unmap_continuous(b->as, b->start + b->size_mapped, mapped);
			b->size_mapped -= mapped;
			size -= mapped;
			pp = b->ilist;
			b->ilist = pp->next;
			b->size_allocated -= pagesize;
			ppage_free(pp);
		}
		else
		{
			unmap_continuous(b->as, b->start + b->size_mapped, size);
			b->size_mapped -= size;
			size = 0;
		}
	}

	return NOERROR;
}

int vm_block_info(struct vm_block *b)
{
	struct ppage *pp = NULL;

	printf("vm_block start:%u allocated:%u mapped:%u\r\n",
				 b->start, b->size_allocated, b->size_mapped);

	for (pp = b->ilist; pp != NULL; pp = pp->next)
		printf("        ppage size:%u start:%u end:%u\r\n", pp->end - pp->start + 1,
					 pp->start, pp->end);
	return NOERROR;
}
