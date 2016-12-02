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

#ifndef CONFIG_H_
# define CONFIG_H_

# define SYSTEM_CLK 40					/* MHz */

# define WINDOWS_NBR 8					/* the number of windows register on the systeme */
# define PSR_INIT 0x10c0
# define PARTITION_SIZE (256 * 1024)	/* 256 Kbytes */
# define RAM_ADDR 0x40000000
# define RAM_SIZE (16 * 1024 * 1024)	/*  16 Mbytes */
# define PAGE_SIZE 4096					/*   4 Kbytes */
# define MAX_CTX_NBR 256

# define IOAREA_ADDR 0x80000000

# define TICK_COUNTER_VALUE 0xFFFF

# define UART1_BAUDRATE 9600

# define DEBUG_LEVELS DL_INFO
# define DEBUG_SECTIONS (DS_CONS | DS_AMBA | DS_TIMER | DS_IRQ | DS_CORE | \
												 DS_MM | DS_TRAPS | DS_PARTITION | DS_SVGA | \
												 DS_SYSCALL | DS_SCHED )

# define PARTITION_NBR 1
# define CONTEXTS_NUMBER PARTITION_NBR
# define FIRST_PARTITION 0

# define VM_STACK_BASE_ADDR 0xA0000000
# define VM_MAX_STACK_SIZE  (5 * PAGE_SIZE)
# define VM_HEAP_BASE_ADDR  0x80000000

# define MAX_PARTITION_THREADS 10

#endif /* ! CONFIG_H_ */
