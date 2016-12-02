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
#include "mm/vm_stacks.h"
#include "mm/ppages.h"
#include "mm/kalloc.h"
#include "mm/mapping.h"
#include "debug.h"

int vm_stack_enlarge(struct stack *s)
{
	struct ppage *pp = NULL;

	pp = ppage_alloc(1);
	if (pp == NULL)
		return -ENOMEM;

	pp->next = s->list;
	s->list = pp;
	s->size++;
	return map_page(s->as, s->start_page - s->size + 1, pp->start, VM_STACKS_FLAGS);
}

int vm_stack_reduce(struct stack *s)
{
	struct ppage *pp = NULL;

	unmap_page(s->as, s->start_page - s->size + 1);
	pp = s->list;
	s->list = pp->next;
	ppage_free(pp);
	s->size--;
	return NOERROR;
}

struct stack *vm_add_stack(struct vm_stacks *vms)
{
	struct stack *s = NULL;

	s = kalloc(sizeof (*s));
	if (s == NULL)
		return NULL;

	s->start_sp   = VM_STACK_BASE_ADDR + (vms->num * VM_MAX_STACK_SIZE) - 0x40;
	s->start_page = s->start_sp / PAGE_SIZE;
	s->size   = 0;
	s->list   = NULL;
	s->next   = vms->list;
	s->as     = vms->as;
	vms->list = s;
	vms->num++;
	return s;
}

int vm_rm_stack(struct vm_stacks *vms, struct stack *s)
{
	struct stack *prev = NULL;
	struct stack *curr = NULL;
	struct ppage *p    = NULL;
	struct ppage *dead = NULL;

	for (curr = vms->list;
			 curr != NULL && curr != s;
			 prev = curr, curr = curr->next)
		continue;

	if (curr == NULL)
		return -EINVAL;

	if (prev == NULL)
		vms->list = curr->next;
	else
		prev->next = curr->next;

	/* unmap and free ppage */
	unmap_continuous(s->as, s->start_page - s->size, s->size);
	for (p = s->list; p != NULL;)
	{
		dead = p;
		p = p->next;
		kfree(dead);
	}

	vms->num--;
	kfree(s);
	return NOERROR;
}
