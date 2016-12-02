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
#include "utils.h"
#include "cons.h"

#include <stdarg.h>

void memset(unsigned char *buf, unsigned char val, unsigned int size)
{
	unsigned int i = 0;

	ASSERT(buf != NULL);

	for (i = 0; i < size; i++)
		*buf++ = val;
}

int printf(const char *format, ...)
{
	va_list arg;

	va_start(arg, format);

	while (*format != '\0')
	{
		if (*format == '%')
		{
			format++;
			switch (*format)
			{
				case 'd':
					if (print_int(va_arg(arg, int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 'u':
					if (print_uint(va_arg(arg, unsigned int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 'x':
					if (print_uint_hex(va_arg(arg, unsigned int)) != NOERROR)
						return ERROR;
					format++;
					break;
				case 's':
					if (print(va_arg(arg, char *)) != NOERROR)
						return ERROR;
					format++;
					break;
				case '%':
					if (print_char('%') != NOERROR)
						return ERROR;
					format++;
					break;
				default:
					if (print_char('%') != NOERROR || print_char(*format++) != NOERROR)
						return ERROR;
					break;
			}
		}
		else if (print_char(*format++) != NOERROR)
			return ERROR;

	}
	va_end(arg);
	return NOERROR;
}
