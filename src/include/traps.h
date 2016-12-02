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

#ifndef TRAPS_H_
# define TRAPS_H_

# include "core.h"

# define TRAP_IRQ_BASE     0x10
# define TRAP_SYSCALL_BASE 0x80

# define HANDLED     1
# define NOT_HANDLED 2

extern vaddr g_restore_sp;

typedef int (*isr_f) (void);

bool trap_set_isr(isr_f isr, unsigned int trap_nbr, bool force);

int  traps_init();

void print_isr();

#endif /* ! TRAPS_H_ */
