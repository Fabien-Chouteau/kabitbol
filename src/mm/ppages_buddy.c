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

static struct ppages_manager g_ppages;

/**
 * Split buddy from order @from to order @to
 */
static
int split_order(unsigned int order)
{
  struct ppage *daddy = g_ppages.list[order];
  struct ppage *b1 = NULL;
  struct ppage *b2 = NULL;

  if ( ! daddy->free)
    return -EINVAL;

  b1 = kalloc(sizeof (*b1));
  if (b1 == NULL)
    return -ENOMEM;

  b2 = kalloc(sizeof (*b2));
  if (b2 == NULL)
  {
    kfree(b2);
    return -ENOMEM;
  }

  /* remove daddy from the list */
  g_ppages.list[order] = daddy->next;
  daddy->next = NULL;

  /* Split daddy */
  daddy->splited = TRUE;

  b1->daddy   = daddy;
  b1->buddy   = b2;
  b1->free    = TRUE;
  b2->splited = FALSE;
  b1->order   = order - 1;
  b1->start   = daddy->start;
  b1->end     = b1->start + (1 << (order - 1)) - 1;

  b2->daddy   = daddy;
  b2->buddy   = b1;
  b2->free    = TRUE;
  b2->splited = FALSE;
  b2->order   = order - 1;
  b2->end     = daddy->end;
  b2->start   = b2->end - (1 << (order - 1)) + 1;

  /* add buddies to the list */
  b1->next = b2;
  b2->next = g_ppages.list[order - 1];
  g_ppages.list[order - 1] = b1;

  return NOERROR;
}

static
int remove_buddy_from_list(struct ppage *b)
{
  struct ppage *curr = NULL;
  struct ppage *prev = NULL;

  for (curr = g_ppages.list[b->order];
       curr != NULL && curr != b;
       prev = curr, curr = curr->next)
    continue;

  if (curr == NULL)
    return -EINVAL;

  if (prev == NULL)
    g_ppages.list[b->order] = b->next;
  else
    prev->next = b->next;

  b->next = NULL;
  return NOERROR;
}

static
int ppage_merge_buddies(struct ppage *b)
{
  int status = NOERROR;
  struct ppage *daddy = NULL;

  while (b->free && b->buddy != NULL && b->buddy->free && ! b->buddy->splited)
  {
    if (b->daddy == NULL || b->daddy != b->buddy->daddy)
      return NOERROR;

    status = remove_buddy_from_list(b);
    if (status != NOERROR)
      return status;
    status = remove_buddy_from_list(b->buddy);
    if (status != NOERROR)
      return status;

    daddy = b->daddy;
    daddy->splited = FALSE;
    daddy->free    = TRUE;

    /* add daddy to his list */
    daddy->next = g_ppages.list[daddy->order];
    g_ppages.list[daddy->order] = daddy;

    /* pdebug(DS_MM, DL_INFO, "ppages : merge buddies from %u to %u\r\n", */
    /*        b->order, daddy->order); */

    /* free buddies */
    kfree(b->buddy);
    kfree(b);

    /* try to merge daddy on next loop */
    b = daddy;
  }
  return NOERROR;
}

struct ppage *ppage_alloc(unsigned int size)
{
  unsigned int order = 0;
  unsigned int i = 0;
  struct ppage * ret = NULL;

  /* find the first 2^i > size */
  for (order = 0; size > (1 << order) && order < MAX_BUDDY_ORDER; order++)
    continue;

  if (order >= MAX_BUDDY_ORDER)
    return NULL;

  for (i = order; g_ppages.list[i] == NULL && i < MAX_BUDDY_ORDER; i++)
    continue;

  if (i == MAX_BUDDY_ORDER)
    return NULL;

  /* Found a good buddy */
  if (i == order)
    goto found;

  /* We have to split */
  for (; i != order; i--)
    if (split_order(i) != NOERROR)
      return NULL;

 found:
  ret = g_ppages.list[order];
  g_ppages.list[order] = ret->next;
  ret->free = FALSE;
  return ret;
}

int ppage_free(struct ppage *b)
{
  if (b->free || b->splited)
    return -EINVAL;

  b->free = TRUE;
  b->next = g_ppages.list[b->order];
  g_ppages.list[b->order] = b;
  /* Buddy ready to merge ? */
  return ppage_merge_buddies(b);
}

/**
 * Try to add maximum buddies in the area.
 * For exemple in the area 0->11. There's a 2^3 and a 2^2 buddy.
 * 0->7 and 8->11.
 */
int ppage_add_area(unsigned int start, unsigned int end)
{
  struct ppage *b = NULL;
  unsigned int order = 0;

  for (order = MAX_BUDDY_ORDER - 1; start <= end;)
  {
    if ((end - start + 1) >= (unsigned int)(1 << order))
    {
      b = kalloc(sizeof (*b));
      if (b == NULL)
        return -ENOMEM;

      b->start   = start;
      b->end     = start + (1 << order) - 1;
      b->daddy   = NULL;
      b->buddy   = NULL;
      b->splited = FALSE;
      b->free    = TRUE;
      b->order   = order;

      /* add to list */
      b->next = g_ppages.list[order];
      g_ppages.list[order] = b;

      pdebug(DS_MM, DL_INFO, "ppages : new buddy in order %u\r\n", order);
      /* new size of the free area */
      start = b->end + 1;
    }
    else
      order--;
  }
  return NOERROR;
}

void ppage_info()
{
  struct ppage *b = NULL;
  int i = 0;
  int order = 0;

  printf("ppages Buddies info\r\n");
  for (order = 0; order < MAX_BUDDY_ORDER; order++)
  {
    for (i = 0, b = g_ppages.list[order]; b != NULL; b = b->next, i++)
      continue;

    printf("ppages : %u free buddies in order %u\r\n", i , order);
  }
}

int ppage_print(struct ppage *p)
{
  return printf("ppage : page start:%u end:%u order:%u free:%s splited:%u "
                "daddy:0x%x buddy:0x%x\r\n", p->start, p->end, p->order,
                p->free ? "y" : "n", p->splited, p->daddy, p->buddy);
}

int ppage_init()
{
  int order = 0;

  for (order = 0; order < MAX_BUDDY_ORDER; order++)
    g_ppages.list[order] = NULL;
  return NOERROR;
}
