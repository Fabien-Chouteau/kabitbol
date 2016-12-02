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
#include "error.h"
#include "stream.h"
#include "syscall.h"

#include <stdarg.h>

static int stream_print_uint(stream_s *s, unsigned int val)
{
	int i = 9;
	char tmp[11];

	if (val == 0)
		return (stream_putc(s, '0'));

	tmp[10] = '\0';

	for (; i >= 0 && val != 0; i--)
	{
		tmp[i] = (val % 10) + '0';
		val = val / 10;
	}

	stream_puts(s, tmp + i + 1);
	return NOERROR;
}

static int stream_print_int(stream_s *s, int val)
{
	if (val == 0)
		return (stream_putc(s, '0'));

	if (val < 0)
		stream_putc(s, '-');
	return stream_print_uint(s, val < 0 ? -val : val);
}

static int stream_print_uint_hex(stream_s *s, unsigned int val)
{
	int i = 7;
	char tmp[9];

	if (val == 0)
		return (stream_putc(s, '0'));

	tmp[8] = '\0';

	for (; i >= 0 && val != 0; i--)
	{
		if ((val % 16) < 10)
			tmp[i] = (val % 16) + '0';
		else
			tmp[i] = (val % 16) + 'A' - 10;
		val = val / 16;
	}

	stream_puts(s, tmp + i + 1);
	return NOERROR;
}

int sprint_format(stream_s *s, const char* format, va_list arg)
{
	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			switch (*format)
			{
				case 'd':
					if (stream_print_int(s, va_arg(arg, int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 'u':
					if (stream_print_uint(s, va_arg(arg, unsigned int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 'x':
					if (stream_print_uint_hex(s, va_arg(arg, unsigned int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 's':
					if (stream_puts(s, va_arg(arg, char *)) != NOERROR)
						return ERROR;
					format++;
					break;
				case '%':
					if (stream_putc(s, '%') != NOERROR)
						return ERROR;
					format++;
					break;
				default:
					if (stream_putc(s, '%') != NOERROR
							|| stream_putc(s, *format++) != NOERROR)
						return ERROR;
					break;
			}
		}
		else if (stream_putc(s, *format++) != NOERROR)
			return ERROR;

	}
	return NOERROR;
}

int sprintf(stream_s *s, const char *format, ...)
{
	va_list  args;
	int      ret = 0;

	va_start (args, format);
	ret = sprint_format(s, format, args);
	va_end (args);

	return ret;
}

int syscall_print_flush(stream_s *s)
{
	return syscall2(SYSCALL_PRINT, (uint32_t)s->buf, (uint32_t)s->len);
}

int printf(const char *format, ...)
{
	static stream_s *s = NULL;
	va_list  args;

	if (s == NULL)
	{
		s = new_stream(syscall_print_flush);
		if (s == NULL)
			return -ENOMEM;
	}
	va_start (args, format);
	sprint_format(s, format, args);
	va_end (args);

	return stream_flush(s);
}
