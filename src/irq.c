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
#include "irq.h"
#include "cons.h"
#include "amba.h"
#include "utils.h"

volatile irq_s *g_irq = NULL;

int irq_init()
{
const amba_dev_s *dev =
	amba_find_device(AMBA_APB_SLAVE, VENDOR_GAISLER, GAISLER_IRQMP);
	if (dev == NULL)
		return ERROR;

	g_irq =
		(irq_s *) amba_iobar_start((unsigned int) dev->masteraddr, dev->addr[1]);

	if (g_irq == NULL)
		return ERROR;

	return NOERROR;
}

int start_cpu(uint8_t cpuid)
{
	outw((uint32_t)&g_irq->mpstat, 1 << cpuid);
	return NOERROR;
}

int start_all_cpu(void)
{
	outw((uint32_t)&g_irq->mpstat, 0xffff);
	return NOERROR;
}
