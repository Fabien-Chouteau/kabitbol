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
#include "mm/ppages.h"
#include "debug.h"
#include "mm/kalloc.h"

struct ppages_manager g_ppages;

/*----------------------------------------------------------------------------*/
/*                                 blocks                                     */

static
int ppages_split_block(struct ppage *curr,
                       unsigned int start,
                       unsigned int end)
{
  if ( ! curr->free || ! (curr->start <= start && curr->end >= end))
    return -EINVAL;

  /* No Split */
  if (curr->start == start && curr->end == end)
    goto isfree;

  /* Split before */
  if (curr->start != start)
  {
    struct ppage *new = NULL;
    new = kalloc(sizeof (*new));
    if (new == NULL)
      return -ENOMEM;
    new->free  = TRUE;
    new->start = curr->start;
    new->end   = start - 1;
    /* Double linked list stuff  */
    new->next  = curr;
    new->prev  = curr->prev;

    if (curr->prev != NULL)
      curr->prev->next = new;
    else
      g_ppages.blocks = new;

    curr->prev = new;
  }

  /* Split after */
  if (curr->end != end)
  {
    struct ppage *new = NULL;
    new = kalloc(sizeof (*new));
    if (new == NULL)
      return -ENOMEM;
    new->free  = TRUE;
    new->start = end + 1;
    new->end   = curr->end;
    /* Double linked list stuff  */
    new->next  = curr->next;
    new->prev  = curr;

    if (curr->next != NULL)
      curr->next->prev = new;

    curr->next = new;
  }

  curr->end   = end;
  curr->start = start;
 isfree:
  curr->free  = FALSE;
  return NOERROR;
}

static
int ppages_merge_block()
{
  struct ppage *curr = NULL;
  struct ppage *dead = NULL;

  for (curr = g_ppages.blocks; curr != NULL;)
  {
    if (curr->free && curr->next != NULL && curr->next->free)
    {
      dead = curr->next;
      if (dead->start != curr->end + 1)
      {
        pdebug(DS_MM, DL_CRITICAL, "There is a tear in the very fabric of the "
               "space time continuum");
        return ERROR;
      }

      curr->end = dead->end;
      curr->next = dead->next;
      if (curr->next != NULL)
        curr->next->prev = curr;
      kfree(dead);
    }
    else
      curr = curr->next;
  }
  return NOERROR;
}

/**
 * @param size Number of page(s).
 *
 */
static
struct ppage *ppages_alloc_block(unsigned int        size)
{
  struct ppage *curr = NULL;
  int status = 0;

  /* First Fit */
  for (curr = g_ppages.blocks; curr != NULL; curr = curr->next)
  {
    if (curr->free && (curr->end - curr->start + 1) >= size)
    {
      status = ppages_split_block(curr, curr->start, curr->start + size - 1);

			/* remove from list */
			if (curr->next != NULL)
				curr->next->prev = curr->prev;

			if (curr->prev != NULL)
				curr->prev->next = curr->next;
			else
				g_ppages.blocks = curr->next;

			if (status == NOERROR)
        return curr;
      else
        pdebug(DS_MM, DL_ERROR, "alloc block: Couldn't split block: %d\n",
							 status);
      return NULL;
    }
  }
  return NULL;
}

static
int ppages_free_block (struct ppage *p)
{
  p->free = TRUE;
  return ppages_merge_block();
}


/*----------------------------------------------------------------------------*/
/*                                   pages                                    */

static
struct ppage *ppages_alloc_page()
{
  struct ppages_page_chunk *curr = NULL;
  struct ppages_page_chunk *prev = NULL;
  struct ppage *ret = NULL;
  unsigned int i = 0;

  for (curr = g_ppages.chunks;
       curr != NULL;
       prev = curr, curr = curr->next)
  {
    if ( ! curr->full)
    {
      ret = curr->free_stack;
      curr->free_stack = ret->next;
			ret->next = NULL;
			ret->prev = NULL;
      curr->stack_size--;

      if (curr->free_stack == NULL)
      {
        curr->full = TRUE;
        curr->stack_size = 0;
      }
      return ret;
    }
  }

  /* need one more chunk */
  struct ppage *zone = NULL;
  zone = ppages_alloc_block(PPAGES_CHUNK_SIZE);
  if (zone == NULL)
  {
    pdebug(DS_MM, DL_ERROR, "page_alloc: Couldn't alloc block of size %u\r\n",
           PPAGES_CHUNK_SIZE);
    return NULL;
  }

  if ((curr = kalloc(sizeof (*curr))) == NULL)
    return NULL;

  curr->zone = zone;
  curr->free_stack = NULL;
  curr->full = FALSE;
  curr->next = NULL;
  if (prev == NULL)
    g_ppages.chunks = curr;
  else
    prev->next = curr;

  /* fill the stack */
  for (i = 0; i < (PPAGES_CHUNK_SIZE - 1); i++)
  {
    struct ppage *new = NULL;
    new = kalloc(sizeof (*new));
    if (new == NULL)
      return NULL;
    new->free = TRUE;
    new->start = zone->start + i;
    new->end = zone->start + i;
    new->prev = NULL;
    new->next = curr->free_stack;
    curr->free_stack = new;
  }
  curr->stack_size = (PPAGES_CHUNK_SIZE - 1);

  /* the last one is returned */
  struct ppage *new = NULL;
  new = kalloc(sizeof (*new));
  if (new == NULL)
    return NULL;
  new->free = FALSE;
  new->start = zone->start + i;
  new->end = zone->start + i;
  new->prev = NULL;
  new->next = NULL;

  return new;
}

static
int ppages_free_page(struct ppage *b)
{
  struct ppages_page_chunk *curr = NULL;
  struct ppages_page_chunk *prev = NULL;

	b->next = NULL;
	b->prev = NULL;

  for (curr = g_ppages.chunks;
       curr != NULL;
       prev = curr, curr = curr->next)
  {
    if (b->start >= curr->zone->start && b->start <= curr->zone->end)
    {
      curr->full = FALSE;
      b->free = TRUE;
      b->next = curr->free_stack;
      curr->free_stack = b;
      curr->stack_size++;

      if (curr->stack_size == PPAGES_CHUNK_SIZE)
      {
        ppages_free_block (curr->zone);

        /* free all pages */
        while (curr->free_stack != NULL)
        {
          b = curr->free_stack;
          curr->free_stack = b->next;
          kfree(b);
        }

        /* remove from chunk list */
        if (prev != NULL)
          prev->next = curr->next;
        else
          g_ppages.chunks = NULL;

        kfree(curr);
      }
      return NOERROR;
    }
  }
  return -EINVAL;
}


/*----------------------------------------------------------------------------*/
/*                                pretty print                                */

int ppages_block_info()
{
  struct ppage *cur = NULL;
  unsigned int i = 0;
  for (i = 0, cur = g_ppages.blocks;
       cur != NULL;
       i++, cur = cur->next)
  {
    printf("Block N°%u\t-> free:%s size:%u start:%u end:%u\r\n", i,
           cur->free ? "y" : "n", cur->end - cur->start + 1,
           cur->start, cur->end);
  }
  return NOERROR;
}

int ppages_chunk_info()
{
  struct ppages_page_chunk *curr = NULL;
  unsigned int i = 0;
  for (i = 0, curr = g_ppages.chunks;
       curr != NULL;
       i++, curr = curr->next)
  {
    printf("Chunk N°%u\t-> full:%s start:%u end:%u free stack size:%u\r\n", i,
           curr->full ? "y" : "n", curr->zone->start, curr->zone->end,
           curr->stack_size);
  }
  return NOERROR;
}

void ppage_info()
{
  printf("ppages Blocks/Chunks info:\r\n");
  ppages_block_info();
  ppages_chunk_info();
}

int ppage_print(struct ppage *p)
{
  return printf("ppage : free:%s size:%u start:%u end:%u\r\n",
								p->free ? "y" : "n", p->end - p->start + 1, p->start, p->end);
}

/*----------------------------------------------------------------------------*/
/*                                 interface                                  */

int ppage_add_area(unsigned int start,
                   unsigned int end)
{
  struct ppage *new = NULL;
  struct ppage *curr = NULL;
  struct ppage *prev = NULL;

	if (start >= end)
		return -EINVAL;

  new = kalloc(sizeof (struct ppage));
  if (new == NULL)
    return -ENOMEM;

  new->start = start;
  new->end   = end;
  new->free  = TRUE;

  for (curr = g_ppages.blocks;
       curr != NULL && curr->start < new->end;
       prev = curr, curr = curr->next)
    continue;

  if (prev == NULL)
  {
    /* head of the list */
    new->next = g_ppages.blocks;
    new->prev = NULL;
    g_ppages.blocks = new;
  }
  else if (curr == NULL)
  {
    /* tail of the list */
    prev->next = new;
    new->prev  = prev;
    new->next  = NULL;
  }
  else
  {
    /* middle of the list */
    new->next  = curr;
    new->prev  = prev;
    prev->next = new;
    curr->prev = new;
  }

  pdebug(DS_MM, DL_INFO, "ppages block new area: start=%u end=%u\r\n",
         start, end);
  return NOERROR;
}

/**
 * @param size Number of page(s).
 *
 */
struct ppage *ppage_alloc(unsigned int size)
{
  if (size == 0)
    return NULL;

  pdebug(DS_MM, DL_INFO, "ppages alloc size:%u\r\n", size);

  if (size == 1)
    return ppages_alloc_page();

  return ppages_alloc_block(size);
}

int  ppage_free(struct ppage *b)
{
  if (b->start == b->end)
    return ppages_free_page(b);
  else
    return ppages_free_block(b);
}

int  ppage_init()
{
  g_ppages.chunks = NULL;
  g_ppages.blocks = NULL;
	return NOERROR;
}
