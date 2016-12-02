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
#include "utils.h"
#include "traps.h"
#include "debug.h"
#include "mm/mm.h"

static const char *at_str[] = {
	"Load from User Data Space",
	"Load from Supervisor Data Space",
	"Load/Execute from User Instruction Space",
	"Load/Execute from Supervisor Instruction Space",
	"Store to User Data Space",
	"Store to Supervisor Data Space",
	"Store to User Instruction Space",
	"Store to Supervisor Instruction Space"
};

static const char *ft_str[] = {
	"None",
	"Invalid address error",
	"Protection error",
	"Privilege violation error",
	"Translation error",
	"Access bus error",
	"Internal error",
	"Reserved"
};

int page_fault_handler(void)
{
	unsigned int faddr = 0;
	unsigned int fstatus = 0;
	int l   = 0;
	int at  = 0;
	int ft  = 0;
	int fav = 0;

	faddr   = asi_get(MM_FAULT_ADDR, MM_ASI_M_MMUREGS);
	fstatus = asi_get(MM_FAULT_STATUS, MM_ASI_M_MMUREGS);

	l   = (fstatus >> 8) & 0x3;
	at  = (fstatus >> 5) & 0x7;
	ft  = (fstatus >> 2) & 0x7;
	fav = (fstatus >> 1) & 0x1;

	pdebug(DS_MM, DL_INFO, "Page Fault:\r\n");
	pdebug(DS_MM, DL_INFO, "level:%d %s\r\n", l, at_str[at], ft_str[ft]);

	if (fav)
		pdebug(DS_MM, DL_INFO, "%s at 0x%x\r\n", ft_str[ft], faddr);
	else
		pdebug(DS_MM, DL_INFO, "%s at invalid\r\n", ft_str[ft]);
	pdebug(DS_MM, DL_ERROR, "Unhandled page fault\r\n");
	return NOT_HANDLED;
}
