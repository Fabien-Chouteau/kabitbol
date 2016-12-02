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
#include "list.h"
#include "mm/kalloc.h"
#include "error.h"

static void list_add_head_node(list_s *head, list_s *node)
{
	node->next = head->next;
	node->next->prev = node;
	node->prev = head;
	head->next = node;
}

static void list_add_tail_node(list_s *head, list_s *node)
{
	node->prev = head->prev;
	node->prev->next = node;
	node->next = head;
	head->prev = node;
}

static void list_remove_node(list_s *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;
}

int list_add_head(list_s *head, void *data)
{
	list_s *node = kalloc(sizeof (*node));

	if (node == NULL)
		return -ENOMEM;
	node->data = data;
	list_add_head_node(head, node);
	return NOERROR;
}

int list_add_tail(list_s *head, void *data)
{
	list_s *node = kalloc(sizeof (*node));

	if (node == NULL)
		return -ENOMEM;
	node->data = data;
	list_add_tail_node(head, node);
	return NOERROR;
}

int list_remove(list_s *head, void *data)
{
	list_s *node;

	for_each_node(node, head)
	{
		if (node->data == data)
		{
			list_remove_node(node);
			kfree(node);
			return NOERROR;
		}
	}
	return ERROR;
}

void *list_find(list_s *head, void *data)
{
	list_s *node;

	for_each_node(node, head)
	{
		if (node->data == node)
			return data;
	}
	return NULL;
}

int list_is_empty(list_s *head)
{
	return head->next == head;
}
