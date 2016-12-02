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

#include "mm/access.h"
#include "mm/mapping.h"
#include "debug.h"
#include "utils.h"

bool check_access(as_s *as, vaddr ptr, uint32_t size, uint32_t flags)
{
	uint32_t first = ptr / PAGE_SIZE;
	uint32_t last  = (ptr + size) / PAGE_SIZE;

	for (; first <= last; first++)
	{
		pte      p     = mapping_walk(as, first * PAGE_SIZE);
		uint8_t  acc   = p & (0x7 << 2);

		if (p == MM_ET_INVALID)
		{
			pdebug(DS_MM, DL_ERROR, "access invalid at 0x%x\r\n", first * PAGE_SIZE);
			return FALSE;
		}

		if (flags & ACCESS_READ
				&& ! ((acc == MM_ACC_R) || (acc == MM_ACC_RW)
							|| (acc == MM_ACC_RWE) || (acc == MM_ACC_RE)
							|| (acc == MM_ACC_R_S_RW)))
		{
			pdebug(DS_MM, DL_ERROR, "bad read access at 0x%x acc:0x%x\r\n",
						 first * PAGE_SIZE, acc >> 2);
			return FALSE;
		}

		if (flags & ACCESS_WRITE
				&& ! ((acc == MM_ACC_RW) || (acc == MM_ACC_RWE)))
		{
			pdebug(DS_MM, DL_ERROR, "bad write access at 0x%x acc:0x%x\r\n",
						 first * PAGE_SIZE, acc >> 2);
			return FALSE;
		}
	}

	return TRUE;
}

uint32_t write_to_as(struct as *as, vaddr dest, char *src, uint32_t size)
{
	uint32_t i    = 0;
	uint32_t addr = 0;

	for (i = 0; i < size; i++)
	{
		addr = (uint32_t)MM_GET_PT_ADDR(mapping_walk(as, dest + i)) + (i % PAGE_SIZE);
		printf("outb(0x%x, 0x%x)\r\n", addr, src[i]);
		if (addr >= RAM_ADDR && addr < RAM_ADDR + RAM_SIZE)
			outb(addr, src[i]);
	}
	return size;
}
