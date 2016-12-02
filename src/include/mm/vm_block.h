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

#ifndef VM_BLOCK_H_
# define VM_BLOCK_H_

# include "mm/ppages.h"

struct vm_block
{
		unsigned int start;
		unsigned int size_allocated;
		unsigned int size_mapped;
		unsigned int flags;
		struct as    *as;
		/* inverted list of pages
		 * first page->end == block->end
		 * last page->start == block->start
		 */
		struct ppage *ilist;
};

int vm_block_enlarge(struct vm_block *b, unsigned int size);
int vm_block_reduce(struct vm_block *b, unsigned int size);
int vm_block_info(struct vm_block *b);

#endif /* ! VM_BLOCK_H_ */
