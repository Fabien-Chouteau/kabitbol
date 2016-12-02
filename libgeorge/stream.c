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

#include "stream.h"
#include "malloc.h"
#include "error.h"
#include "misc.h"

int stream_putc(stream_s *s, unsigned char c)
{

	if (s->alloc < s->len + 1)
	{
		s->alloc += STREAM_ALLOC_SIZE;
		if ((s->buf = realloc(s->buf, s->alloc)) == NULL)
			return -ENOMEM;
	}
	s->buf[s->len++] = c;
	return NOERROR;
}

int stream_puts(stream_s *s, char *str)
{
	for (; *str != '\0'; str++)
		stream_putc(s, (unsigned char)*str);

	return NOERROR;
}

int stream_flush(stream_s *s)
{
	int ret = 0;

	ret = s->flush(s);
	free(s->buf);
	s->buf   = NULL;
	s->alloc = 0;
	s->len   = 0;
	return ret;
}

stream_s *new_stream(flush_stream_f flush)
{
	stream_s *ret = NULL;

	if ((ret = zalloc(sizeof (*ret))) == NULL)
		return NULL;

	ret->flush = flush;
	ret->len   = 0;
	ret->alloc = 0;

	return ret;
}
