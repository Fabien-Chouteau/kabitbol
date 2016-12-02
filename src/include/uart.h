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

#ifndef UART_H_
# define UART_H_


/* DEFINE FROM RTEMS */

/*
 *  The following defines the bits in the LEON UART Status Registers.
 */

# define UART_STATUS_DR   0x00000001	/* Data Ready */
# define UART_STATUS_TSE  0x00000002	/* TX Send Register Empty */
# define UART_STATUS_THE  0x00000004	/* TX Hold Register Empty */
# define UART_STATUS_BR   0x00000008	/* Break Error */
# define UART_STATUS_OE   0x00000010	/* RX Overrun Error */
# define UART_STATUS_PE   0x00000020	/* RX Parity Error */
# define UART_STATUS_FE   0x00000040	/* RX Framing Error */
# define UART_STATUS_ERR  0x00000078	/* Error Mask */


/*
 *  The following defines the bits in the LEON UART Status Registers.
 */

# define UART_CTRL_RE     0x00000001	/* Receiver enable */
# define UART_CTRL_TE     0x00000002	/* Transmitter enable */
# define UART_CTRL_RI     0x00000004	/* Receiver interrupt enable */
# define UART_CTRL_TI     0x00000008	/* Transmitter interrupt enable */
# define UART_CTRL_PS     0x00000010	/* Parity select */
# define UART_CTRL_PE     0x00000020	/* Parity enable */
# define UART_CTRL_FL     0x00000040	/* Flow control enable */
# define UART_CTRL_LB     0x00000080	/* Loop Back enable */

/* scaler = (((system_clk*10)/(baudrate*8))-5)/10 */
# define UART_SCALER(baudrate) ((((SYSTEM_CLK * 10) / (baudrate * 8)) - 5 ) / 10)

typedef struct uart
{
	volatile unsigned int data;
	volatile unsigned int status;
	volatile unsigned int ctrl;
	volatile unsigned int scaler;
} uart_s;

int uart_init(void);

int uart_1_putc(char c);

#endif /* ! UART_H_ */
