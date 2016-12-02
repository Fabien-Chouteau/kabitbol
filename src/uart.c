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
#include "config.h"
#include "amba.h"
#include "uart.h"

volatile uart_s *g_uart1 = NULL;

int uart_init(void)
{
	const amba_dev_s *dev =
		amba_find_device(AMBA_APB_SLAVE, VENDOR_GAISLER, GAISLER_APBUART);
	if (dev == NULL)
		return ERROR;

	g_uart1 =
		(uart_s *) amba_iobar_start((unsigned int) dev->masteraddr, dev->addr[1]);

	if (g_uart1 == NULL)
		return ERROR;
	g_uart1->ctrl = UART_CTRL_TE;	/* recieve transmit enable */
	g_uart1->status = 0;
	return NOERROR;
}

int uart_1_putc(char c)
{
	if (g_uart1 == NULL)
		return ERROR;

	while ((inw((uint32_t) &(g_uart1->status)) & UART_STATUS_THE) == 0)
		continue;

	outb((uint32_t) &(g_uart1->data), c);

	return NOERROR;
}
