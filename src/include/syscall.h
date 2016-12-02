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

#ifndef SYSCALL_H_
# define SYSCALL_H_

# define SYSCALL_TRAP 0x2

# define SYSCALL_KIKOU     0
# define SYSCALL_PRINT     1
# define SYSCALL_HEAP_INFO 2

# define trap_is_syscall(trap_nbr) ((trap_nbr) == (TRAP_SYSCALL_BASE + SYSCALL_TRAP))

int syscall_init(void);

#endif /* ! SYSCALL_H_ */
