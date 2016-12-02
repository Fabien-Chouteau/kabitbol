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

#ifndef LIST_H_
# define LIST_H_

typedef struct list list_s;

struct list
{
		void   *data;
		list_s *next;
		list_s *prev;
};

# define DECLARE_LIST(name) list_s name = {.next = &name,	\
																					 .prev = &name, \
																					 .data = NULL}

int list_add_head(list_s *head, void *data);
int list_add_tail(list_s *head, void *data);
int list_remove(list_s *head, void *data);
void *list_find(list_s *head, void *data);
int list_is_empty(list_s *head);

# define for_each_node(node, head)													\
	for ((node) = (head)->next; (node) != (head); (node) = (node)->next)

# define for_each(elt, parc, head)												\
	for (parc = head->next, elt = parc->data;								\
			 parc != head; parc = parc->next, elt = parc->data)

# define for_each_safe_node(node, tmp, head)							\
	for (node = head->next, tmp = head->next->next;					\
			 node != head; node = tmp, tmp = tmp->next)

# define for_each_safe(elt, node, tmp, head)												\
	for (node = head->next, tmp = head->next->next, elt = node->data;	\
			 node != head; node = tmp, tmp = tmp->next, elt = node->data)

#endif	/* ! LIST_H_ */
