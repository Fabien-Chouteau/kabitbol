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

#ifndef STREAM_H_
# define STREAM_H_

# define STREAM_ALLOC_SIZE 512

struct stream ;
typedef struct stream stream_s;

typedef int (*flush_stream_f)(stream_s *s);

struct stream
{
		unsigned int   len;
		unsigned int   alloc;
		char           *buf;
		flush_stream_f flush;
};

int stream_putc(stream_s *s, unsigned char c);
int stream_puts(stream_s *s, char *str);
int stream_flush(stream_s *s);
stream_s *new_stream(flush_stream_f flush);

#endif	/* ! STREAM_H_ */
