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

#ifndef TIMER_H_
# define TIMER_H_

# define TIMER_CTRL_EN (1 << 0)	/* Enable  */
# define TIMER_CTRL_RS (1 << 1)	/* Restart */
# define TIMER_CTRL_LD (1 << 2)	/* Load */
# define TIMER_CTRL_IE (1 << 3)	/* Interrupt enable */
# define TIMER_CTRL_IP (1 << 4)	/* Interrupt Pending */
# define TIMER_CTRL_CH (1 << 5)	/* Chain */
# define TIMER_CTRL_DH (1 << 6)	/* Debug Halt */

typedef struct timer_n_timer
{
	volatile unsigned int cnt_val;
	volatile unsigned int cnt_reload_val;
	volatile unsigned int cntl;
	volatile unsigned int unused;
} timer_n_timer_s;

typedef struct timer
{
	volatile unsigned int scaler_val;
	volatile unsigned int scaler_reload_val;
	volatile unsigned int conf;
	volatile unsigned int unused;
	volatile timer_n_timer_s timers[8];
} timer_s;

extern unsigned int g_ticks;

# define timer_get_irq(conf) (((conf) >> 3) & 0x1F)

# define timer_get_timers_nbr(conf) ((conf) & 0x7)

int timer_init(void);

int timer_isr(void);

#endif /* ! TIMER_H_ */
