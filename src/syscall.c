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

#include "syscall.h"
#include "traps.h"
#include "core.h"
#include "context.h"
#include "asm.h"
#include "cons.h"
#include "utils.h"
#include "debug.h"
#include "scheduler.h"

#include "mm/vm_manager.h"

#include "syscall_nbr.h"

int syscall_isr(void)
{
	uint8_t  *ctx    = (uint8_t *)g_restore_sp;
	uint32_t syscall = *(uint32_t *)(ctx - I0_OFFSET);
	uint32_t arg1    = *(uint32_t *)(ctx - I1_OFFSET);
	uint32_t arg2    = *(uint32_t *)(ctx - I2_OFFSET);
	uint32_t arg3    = *(uint32_t *)(ctx - I3_OFFSET);
	uint32_t arg4    = *(uint32_t *)(ctx - I4_OFFSET);
	uint32_t arg5    = *(uint32_t *)(ctx - I5_OFFSET);
	int32_t  ret     = 0;

	pdebug(DS_SYSCALL, DL_INFO, "Syscall %u (0x%x, 0x%x, 0x%x, 0x%x, 0x%x)\r\n",
				 syscall, arg1, arg2, arg3, arg4, arg5);

	switch (syscall)
	{
		case SYSCALL_KIKOU:
			printf("Kikou from ctx %u\r\n", g_curr_th->part->context_nbr);
			ret = NOERROR;
			break;

		case SYSCALL_PRINT:
			ret = syscall_print(arg1, arg2);
			break;

		case SYSCALL_SBRK:
			ret = syscall_sbrk(g_curr_th->part->as, arg1);
			break;

		case SYSCALL_THREAD_ADD:
			ret = partition_add_thread(g_curr_th->part, (vaddr)arg1, arg2);
			break;

		case SYSCALL_THREAD_SET_STATE:
			ret = thread_set_state(g_curr_th->part, (vaddr)arg1, arg2);
			break;

		default:
			ret = -EUNIMPL;
			break;
	}

	pdebug(DS_SYSCALL, DL_INFO, "Syscall %u return (int32_t)%d  (uint32_t)%u 0x%x\r\n",
				 syscall, ret, ret, ret);
	*( int32_t *)(ctx - I0_OFFSET) = ret;
	g_curr_th->pc  += 4; // skip "ta" instruction
	g_curr_th->npc += 4;
	return HANDLED;
}

int syscall_init(void)
{
	if (!trap_set_isr
			(syscall_isr, TRAP_SYSCALL_BASE + SYSCALL_TRAP, FALSE))
		return ERROR;

	return NOERROR;
}
