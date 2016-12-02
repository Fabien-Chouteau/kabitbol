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
#include "psr.h"
#include "utils.h"
#include "context.h"
#include "partition.h"
#include "debug.h"
#include "irq.h"
#include "syscall.h"
#include "scheduler.h"
#include "asm.h"

isr_f g_isr_tab[256] = { 0 };

unsigned int g_window_underflow = 0;
unsigned int g_window_overflow = 0;

vaddr g_restore_sp = 0;

int traps_init()
{
	/* Clear all isr */
	memset((unsigned char *) g_isr_tab, 0x0, sizeof (g_isr_tab));
	return NOERROR;
}

bool trap_set_isr(isr_f isr, unsigned int trap_nbr, bool force)
{
	ASSERT(trap_nbr < 256);

	if (g_isr_tab[trap_nbr] == NULL || force)
	{
		g_isr_tab[trap_nbr] = isr;
		pdebug(DS_TRAPS, DL_INFO, "New isr registered for trap:0x%x -> 0x%x\r\n",
					 trap_nbr, isr);
		return TRUE;
	}
	return FALSE;
}

void trap_handler(unsigned int pc,
									unsigned int npc,
									unsigned int psr,
									unsigned int trap_nb,
									unsigned int restore_counter,
									unsigned int stack_pointer)
{
	ASSERT(trap_nbr < 256);

	/* save thread information */
	if (g_curr_th != NULL)
	{
		g_curr_th->pc  = pc;	/* vaddr */
		g_curr_th->npc = npc;	/* vaddr */
		g_curr_th->sp  = stack_pointer;	/* vaddr */
		g_curr_th->restore_counter = restore_counter;
		g_restore_sp = stack_pointer;
	}

	if (g_isr_tab[trap_nb] != NULL && g_isr_tab[trap_nb] () == HANDLED)
	{
		context_switch();
		g_restore_sp = g_curr_th->sp;

		uint8_t *ctx = (uint8_t *)g_restore_sp;
		*(uint32_t *)(ctx - I6_OFFSET)          = g_curr_th->sp;
		*(uint32_t *)(ctx - PC_OFFSET)          = g_curr_th->pc;
		*(uint32_t *)(ctx - NPC_OFFSET)         = g_curr_th->npc;
		*(uint32_t *)(ctx - RESTORE_CNT_OFFSET) = g_curr_th->restore_counter;

		if ( ! g_curr_th->started)
		{
			*(uint32_t *)(ctx - I0_OFFSET)          = g_curr_th->data;
			g_curr_th->started = TRUE;
		}
		return;
	}
	else
	{
		psr_disable_traps();
		pdebug(DS_TRAPS, DL_ERROR,
					 "Unhandled trap: 0x%x %%PSR=%x %%PC=%x %%nPC=%x\n\r", trap_nb, psr,
					 pc, npc);
		psr_print(DS_TRAPS, DL_CRITICAL, psr);
		while (1)
			continue;
		return;											/* avoid warning */
	}
}

void print_isr()
{
	int i = 0;

	printf("Traps table:0x%x\r\n", g_isr_tab);
	printf("Traps list:\r\n");
	for (i = 0; i < 256; i++)
		if (g_isr_tab[i] != NULL)
			printf("trap 0x%x isr 0x%x\r\n", i, g_isr_tab[i]);
}
