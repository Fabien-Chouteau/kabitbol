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

#ifndef IRQ_H_
# define IRQ_H_

typedef struct irq
{
	volatile unsigned int ilevel;
	volatile unsigned int ipend;
	volatile unsigned int iforce;
	volatile unsigned int iclear;
	volatile unsigned int mpstat;
	volatile unsigned int notused01;
	volatile unsigned int notused02;
	volatile unsigned int notused03;
	volatile unsigned int notused10;
	volatile unsigned int notused11;
	volatile unsigned int notused12;
	volatile unsigned int notused13;
	volatile unsigned int notused20;
	volatile unsigned int notused21;
	volatile unsigned int notused22;
	volatile unsigned int notused23;
	volatile unsigned int mask[16];
	volatile unsigned int force[16];
} irq_s;


extern volatile irq_s *g_irq;


/*
	Macros for IQR
*/

# define unmask_irq(irq_nbr) outw((uint32_t)&(g_irq->mask[0]),\
																	inw((uint32_t)&(g_irq->mask[0]))\
																	| (1 << (irq_nbr)))

# define force_irq(irq_nbr) outw((uint32_t)&(g_irq->force[0]), (1 << (irq_nbr)))

# define ack_irq(irq_nbr) outw((uint32_t)&(g_irq->iclear), (1 << (irq_nbr)))

int irq_init();
int start_cpu(uint8_t cupid);
int start_all_cpu(void);

#endif /* ! IRQ_H_ */
