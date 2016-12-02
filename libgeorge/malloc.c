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

#include "misc.h"
#include "syscall.h"
#include "error.h"

void *sbrk(unsigned int size)
{
	return (void *)syscall1(SYSCALL_SBRK, size);
}

void *malloc(unsigned int size)
{
	size += 4 - (size % 4);
	return (void *)sbrk(size);
}

void *zalloc(unsigned int size)
{
	char *ret = malloc(size);
	unsigned int  i    = 0;

 	if (ret == NULL)
		return NULL;

	for (i = 0; i < size; i++)
		ret[i] = 0;

	return (void *)ret;
}

void free(void *p)
{
	return;
}

void *realloc(void *p, unsigned int size)
{
	char         *ret = malloc(size);
	char         *src = p;
	unsigned int i    = 0;

 	if (ret == NULL)
		return NULL;

	/* so ugly !!! (and outside fisrt alloc...) */
	if (p != NULL)
		for (i = 0; i < size; i++)
			ret[i] = src[i];

	return (void *)ret;
}
