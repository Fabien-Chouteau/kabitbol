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
#include "amba.h"
#include "timer.h"
#include "cons.h"
#include "irq.h"
#include "traps.h"
#include "psr.h"
#include "config.h"
#include "utils.h"
#include "debug.h"
#include "scheduler.h"
#include "context.h"

volatile timer_s *g_timer = NULL;

unsigned int g_ticks = 0;

int timer_init(void)
{
	const amba_dev_s *dev =
		amba_find_device(AMBA_APB_SLAVE, VENDOR_GAISLER, GAISLER_GPTIMER);

	if (dev == NULL)
		return ERROR;

	g_timer =
		(timer_s *) amba_iobar_start((unsigned int) dev->masteraddr, dev->addr[1]);

	if (g_timer == NULL)
		return ERROR;

	unsigned int irq = timer_get_irq(g_timer->conf);

	pdebug(DS_TIMER, DL_INFO, "TIMER : bar = 0x%x\n\r", (unsigned int) g_timer);
	pdebug(DS_TIMER, DL_INFO, "TIMER : timers nbr = %u\n\r",
				 timer_get_timers_nbr(g_timer->conf));
	pdebug(DS_TIMER, DL_INFO, "TIMER : irq = %u\n\r", irq);
	pdebug(DS_TIMER, DL_INFO, "TIMER : scal = %u\n\r", g_timer->scaler_val);
	pdebug(DS_TIMER, DL_INFO, "TIMER : reload scal = %u\n\r",
				 g_timer->scaler_val);

	/* enable 1st timer and interrupt */
	g_timer->scaler_val = 1;
	g_timer->scaler_reload_val = 1;
	g_timer->timers[0].cnt_val = TICK_COUNTER_VALUE;
	g_timer->timers[0].cnt_reload_val = TICK_COUNTER_VALUE;
	g_timer->timers[0].cntl =
		TIMER_CTRL_EN | TIMER_CTRL_RS | TIMER_CTRL_LD | TIMER_CTRL_IE;

	if (!trap_set_isr(timer_isr, TRAP_IRQ_BASE + irq, FALSE))
		return ERROR;

	g_ticks = 0;
	unmask_irq(irq);
	ack_irq(irq);
	return NOERROR;
}


int timer_isr(void)
{
	g_ticks++;

	schedule();
	ack_irq(timer_get_irq(inw((uint32_t) &(g_timer->conf))));
	return HANDLED;
}
