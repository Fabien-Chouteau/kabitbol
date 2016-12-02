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
#include "utils.h"
#include "mm/mm.h"
#include "mm/kalloc.h"
#include "mm/ppages.h"
#include "mm/vm_block.h"
#include "mm/pt_alloc.h"
#include "mm/mapping.h"
#include "debug.h"

int mm_init(void)
{
	unsigned int ppage_area_start = 0;
	unsigned int ppage_area_end = 0;


	trap_set_isr(page_fault_handler, 0x1, TRUE); /* instruction_access_exception */
	trap_set_isr(page_fault_handler, 0x9, TRUE); /* data_access_exception */

	trap_set_isr(page_fault_handler, 0x3C, TRUE); /* instruction_access_MMU_miss */
	trap_set_isr(page_fault_handler, 0x2C, TRUE); /* data_access_MMU_miss */

	trap_set_isr(page_fault_handler, 0x21, TRUE); /* instruction_access_error */
	trap_set_isr(page_fault_handler, 0x29, TRUE); /* data_access_error */


	if (kalloc_init() != NOERROR)
		return -ERROR;

	if (ppage_init() != NOERROR)
		return -ERROR;

	if (pt_alloc_init() != NOERROR)
		return -ERROR;

	if (mapping_init() != NOERROR)
		return -ERROR;

	/* set the physical page area */
	ppage_area_start = ((unsigned int) g_brk / PAGE_SIZE) + 2;
	ppage_area_end = ((RAM_ADDR + RAM_SIZE) / PAGE_SIZE) - 1;
	if (ppage_add_area(ppage_area_start, ppage_area_end) != NOERROR)
		return -ERROR;

	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	/* No more allocation before MMU is enabled */
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

	printf("end of ram %u 0x%x %u\r\n", (RAM_ADDR + RAM_SIZE), (RAM_ADDR + RAM_SIZE), (RAM_ADDR + RAM_SIZE) / PAGE_SIZE);
	printf("g_brk %u 0x%x %u\r\n", g_brk, g_brk, (unsigned int)g_brk / PAGE_SIZE);

	kalloc_switch();

	/* set context table */
	asi_set(MM_CTXTBL_PTR, ((unsigned int) g_contexts_tab) >> 4,
					MM_ASI_M_MMUREGS);
	/* set context number */
	asi_set(MM_CTX_REG, 0, MM_ASI_M_MMUREGS);

	mm_ctrl_print(DS_MM, DL_INFO, asi_get(MM_CTRL_REG, MM_ASI_M_MMUREGS));

	/* enable MMU */
	asm volatile ("flush");
	asi_set(MM_CTRL_REG, 0x1, MM_ASI_M_MMUREGS);
	pdebug(DS_MM, DL_INFO, "MMU enabled\r\n");

	return NOERROR;
}
