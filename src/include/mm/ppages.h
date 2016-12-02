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

#ifndef PPAGES_H_
# define PPAGES_H_


/**
 * All allocator export the same interface with this ppage struct.
 */
struct ppage;
struct ppage
{
	/* Public */
	unsigned free:1;
	unsigned int start;
	unsigned int end;
	/* for free any needed list */
	struct ppage *next;

	/* Private */
# ifdef PPAGES_ALLOC_BUDDY
	unsigned splited:1;
	unsigned int order;
	struct ppage *buddy;
	struct ppage *daddy;
# else									/* BLOCKS ALLOC */
	struct ppage *prev;
# endif
};



# ifdef PPAGES_ALLOC_BUDDY

/* BUDDY ALLOCATOR */
#  define MAX_BUDDY_ORDER 10

struct ppages_manager
{
	struct ppage *list[MAX_BUDDY_ORDER];
};

# else /* CHUNK ALLOCATOR */


#  define PPAGES_OWNER_NOBODY (-2)
#  define PPAGES_OWNER_KERNEL (-1)

#  define PPAGES_CHUNK_SIZE 32

struct ppages_page_chunk;
struct ppages_page_chunk
{
	unsigned full:1;
	struct ppage *zone;
	unsigned int stack_size;
	struct ppage *free_stack;
	struct ppages_page_chunk *next;
};

struct ppages_manager
{
	struct ppage *blocks;
	struct ppages_page_chunk *chunks;
};

# endif

/* Common Interface */
struct ppage *ppage_alloc(unsigned int size);
int ppage_free(struct ppage *b);
int ppage_add_area(unsigned int start, unsigned int end);
void ppage_info();
int ppage_init();
int ppage_print(struct ppage *p);

#endif /* ! PPAGES_H_ */
