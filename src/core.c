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
#include "traps.h"
#include "cons.h"
#include "amba.h"
#include "timer.h"
#include "irq.h"
#include "uart.h"
#include "partition.h"
#include "mm/mm.h"
#include "svga.h"
#include "psr.h"
#include "utils.h"
#include "syscall.h"
#include "debug.h"
#include "context.h"
#include "scheduler.h"

extern void start_first_partition(unsigned int pc,
																	unsigned int npc, unsigned int sp);

void kabitbol_init()
{

	psr_enable_traps();
	psr_disable_interupt();
	amba_init();
	uart_init();
	traps_init();

	if (cons_init() != NOERROR)
		pdebug(DS_CONS, DL_ERROR, "Cons init failed\n\r");	/* hum ?!? */
	else
		pdebug(DS_CONS, DL_INFO, "Cons init ok\n\r");

        pdebug(DS_CONS, DL_INFO, "Kabitbol Copyright (C) 2009-2011 Fabien Chouteau\n");

	amba_print();

	unsigned int value = 0;
  asm volatile ("rd %%asr17, %0;\n"
                : "=r" (value));

	pdebug(DS_IRQ, DL_INFO, "CPU nbr:%d\n\r", value >> 28);


	if (irq_init() != NOERROR)
		pdebug(DS_IRQ, DL_CRITICAL, "IRQ init failed\n\r");
	else
		pdebug(DS_IRQ, DL_INFO, "IRQ init ok\n\r");

	if (syscall_init() != NOERROR)
		pdebug(DS_SYSCALL, DL_CRITICAL, "SysCall init failed\n\r");
	else
		pdebug(DS_SYSCALL, DL_INFO, "SysCall init ok\n\r");

	if (svga_init() != NOERROR)
		pdebug(DS_SVGA, DL_ERROR, "VGA init failed\n\r");
	else
		pdebug(DS_SVGA, DL_INFO, "VGA init ok\n\r");

	if (timer_init() != NOERROR)
		pdebug(DS_TIMER, DL_CRITICAL, "Timer init failed\n\r");
	else
		pdebug(DS_TIMER, DL_INFO, "Timer init ok\n\r");

	if (mm_init() != NOERROR)
		pdebug(DS_MM, DL_CRITICAL, "MM init failed\n\r");
	else
		pdebug(DS_MM, DL_INFO, "MM init ok\n\r");

	if (partition_init() != NOERROR)
                pdebug(DS_PARTITION, DL_CRITICAL, "Partition init failed\n\r");
	else
                pdebug(DS_PARTITION, DL_INFO, "Partition init ok\n\r");

	print_isr();

	/* start_all_cpu(); */

	/* Test Enable cache (see LEON3_DS.pdf) */
	asm volatile ("flush; set 0x81000f, %g1; sta %g1, [%g0] 2;");

	schedule();
	context_switch();

	/* jump on first partition */
	g_curr_th->started = TRUE;
	start_first_partition(g_curr_th->pc,
												g_curr_th->npc,
												g_curr_th->sp);
}
