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
#include "cons.h"
#include "uart.h"
#include "mm/access.h"
#include "context.h"
#include "debug.h"
#include "scheduler.h"

int cons_init()
{
unsigned int *bar = (unsigned int *) 0x80000100;

	bar[3] = 0x81;

	return NOERROR;
}

int print_char(char c)
{
	return uart_1_putc(c);
}

int print(const char *str)
{
	for (; *str != '\0'; str++)
		if (print_char(*str) != NOERROR)
			return ERROR;

	return NOERROR;
}

int print_int(int val)
{
	if (val == 0)
		return (print_char('0'));

	if (val < 0)
		print_char('-');
	return print_uint(val < 0 ? -val : val);
}

int print_uint(unsigned int val)
{
	int i = 9;
	char tmp[11];

	if (val == 0)
		return (print_char('0'));

	tmp[10] = '\0';

	for (; i >= 0 && val != 0; i--)
	{
		tmp[i] = (val % 10) + '0';
		val = val / 10;
	}

	print(tmp + i + 1);
	return NOERROR;
}

int print_uint_hex(unsigned int val)
{
	int i = 7;
	char tmp[9];

	if (val == 0)
		return (print_char('0'));

	tmp[8] = '\0';

	for (; i >= 0 && val != 0; i--)
	{
		if ((val % 16) < 10)
			tmp[i] = (val % 16) + '0';
		else
			tmp[i] = (val % 16) + 'A' - 10;
		val = val / 16;
	}

	print(tmp + i + 1);
	return NOERROR;
}

int syscall_print(uint32_t ptr, uint32_t len)
{
	uint32_t i = 0;
	unsigned char *c = (unsigned char *)ptr;

	if ( ! check_access(g_curr_th->part->as, ptr, len, ACCESS_READ))
	{
		pdebug(DS_SYSCALL, DL_ERROR, "syscall_print access ERROR\r\n");
		return -EFAULT;
	}

	for (i = 0; i < len; i++)
		if (print_char(c[i]) != NOERROR)
			return ERROR;

	return len;
}
