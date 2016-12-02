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

#include "kikou.h"
#include "printf.h"
#include "syscall.h"
#include "malloc.h"
#include "thread_state.h"

void test_thread(char *s)
{
	printf("arg:0x%x\r\n", s);
	printf(s);
	while (1)
		continue;
}

int __init(void)
{
	int ret = 0;
	int32_t thid = 1;

	ret = printf("========================== start\r\n");
	do
	{
		thid = syscall2(SYSCALL_THREAD_ADD, (uint32_t)test_thread,
				(uint32_t)"==== New thread ====\r\n");
		if (thid >= 0)
		  syscall2(SYSCALL_THREAD_SET_STATE, thid, THREAD_STATE_RUN);
	} while (thid >= 0);

	while (1)
		continue;
  return ret;
}
