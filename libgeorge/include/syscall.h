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

# include "syscall_nbr.h"
# include "types.h"

uint32_t syscall0(uint32_t code);

uint32_t syscall1(uint32_t code,
									uint32_t arg1);

uint32_t syscall2(uint32_t code,
									uint32_t arg1,
									uint32_t arg2);

uint32_t syscall3(uint32_t code,
									uint32_t arg1,
									uint32_t arg2,
									uint32_t arg3);

uint32_t syscall4(uint32_t code,
									uint32_t arg1,
									uint32_t arg2,
									uint32_t arg3,
									uint32_t arg4);

uint32_t syscall5(uint32_t code,
									uint32_t arg1,
									uint32_t arg2,
									uint32_t arg3,
									uint32_t arg4,
									uint32_t arg5);


#endif	/* ! SYSCALL_H_ */
