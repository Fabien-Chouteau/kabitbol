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
#include "amba.h"
#include "cons.h"
#include "utils.h"
#include "debug.h"

/* global structure for amba devices */
static amba_dev_table_s g_amba_devs;

int amba_init()
{
unsigned int ioarea = LEON3_IO_AREA;
unsigned int *cfg_area = 0;
int i = 0;

	/* init global amba struct */
	g_amba_devs.devnbr = 0;
	g_amba_devs.ioarea = (unsigned int *) ioarea;

	/* SCAN AHB MASTERS */
	cfg_area = (unsigned int *) (ioarea | AMBA_CONF_AREA);
	for (i = 0; i < 16; i++)
	{
		if (*cfg_area != 0)
		{
			amba_add_dev(&g_amba_devs, cfg_area, AMBA_AHB_MASTER, 0);
		}
		cfg_area += AMBA_AHB_CONF_WORDS;
	}

	cfg_area =
		(unsigned int *) (ioarea | AMBA_CONF_AREA | AMBA_AHB_SLAVE_CONF_AREA);
	/* SCAN AHB SLAVES */
	for (i = 0; i < 16; i++)
	{
		if (*cfg_area != 0)
		{
			amba_add_dev(&g_amba_devs, cfg_area, AMBA_AHB_SLAVE, 0);
		}
		cfg_area += AMBA_AHB_CONF_WORDS;
	}


	for (i = 0; i < g_amba_devs.devnbr; i++)
	{
		if (g_amba_devs.dev[i].type == AMBA_AHB_SLAVE)
		{
unsigned int conf_word = g_amba_devs.dev[i].addr[0];
unsigned int mbar = g_amba_devs.dev[i].addr[4];
int j = 0;

			if (amba_get_vendor(conf_word) == VENDOR_GAISLER
					&& amba_get_device(conf_word) == GAISLER_APBMST)
			{
unsigned int apbmst = amba_membar_start(mbar);
				cfg_area = (unsigned int *) (apbmst | AMBA_CONF_AREA);

				for (j = 0; j < AMBA_APB_SLAVES_NBR; j++)
				{
					if (*cfg_area != 0)
					{
						amba_add_dev(&g_amba_devs, cfg_area, AMBA_APB_SLAVE, apbmst);
					}
					cfg_area += AMBA_APB_CONF_WORDS;
				}
			}
		}
	}

	return NOERROR;
}

const amba_dev_s *amba_find_device(amba_dev_type_e type, unsigned int vendor,
																	 unsigned int device)
{
	int i = 0;

	for (i = 0; i < g_amba_devs.devnbr; i++)
	{
	unsigned int conf = g_amba_devs.dev[i].addr[0];
		if (g_amba_devs.dev[i].type == type
				&& amba_get_vendor(conf) == vendor && amba_get_device(conf) == device)
			return g_amba_devs.dev + i;
	}

	return NULL;
}


static const char *amba_type2str(amba_dev_type_e type)
{
	switch (type)
	{
	 case AMBA_AHB_MASTER:
		 return "ahb_master";
		 break;
	 case AMBA_AHB_SLAVE:
		 return "ahb_slave";
		 break;
	 case AMBA_APB_SLAVE:
		 return "apb_slave";
		 break;
	}
	return "";
}

static device_name_s gaisler_device_name[] = {
	{GAISLER_LEON3, "GAISLER_LEON3"},
	{GAISLER_LEON3DSU, "GAISLER_LEON3DSU"},
	{GAISLER_ETHAHB, "GAISLER_ETHAHB"},
	{GAISLER_ETHMAC, "GAISLER_ETHMAC"},
	{GAISLER_APBMST, "GAISLER_APBMST"},
	{GAISLER_AHBUART, "GAISLER_AHBUART"},
	{GAISLER_SRCTRL, "GAISLER_SRCTRL"},
	{GAISLER_SDCTRL, "GAISLER_SDCTRL"},
	{GAISLER_APBUART, "GAISLER_APBUART"},
	{GAISLER_IRQMP, "GAISLER_IRQMP"},
	{GAISLER_AHBRAM, "GAISLER_AHBRAM"},
	{GAISLER_GPTIMER, "GAISLER_GPTIMER"},
	{GAISLER_PCITRG, "GAISLER_PCITRG"},
	{GAISLER_PCISBRG, "GAISLER_PCISBRG"},
	{GAISLER_PCIFBRG, "GAISLER_PCIFBRG"},
	{GAISLER_PCITRACE, "GAISLER_PCITRACE"},
	{GAISLER_AHBTRACE, "GAISLER_AHBTRACE"},
	{GAISLER_ETHDSU, "GAISLER_ETHDSU"},
	{GAISLER_PIOPORT, "GAISLER_PIOPORT"},
	{GAISLER_AHBJTAG, "GAISLER_AHBJTAG"},
	{GAISLER_ATACTRL, "GAISLER_ATACTRL"},
	{GAISLER_VGA, "GAISLER_VGA"},
	{GAISLER_SVGA, "GAISLER_SVGA"},
	{GAISLER_KBD, "GAISLER_KBD"},
	{GAISLER_L2TIME, "GAISLER_L2TIME"},
	{GAISLER_L2C, "GAISLER_L2C"},
	{GAISLER_PLUGPLAY, "GAISLER_PLUGPLAY"},
	{GAISLER_SPW, "GAISLER_SPW"},
	{GAISLER_SPW2, "GAISLER_SPW2"},
	{GAISLER_EHCI, "GAISLER_EHCI"},
	{GAISLER_UHCI, "GAISLER_UHCI"},
	{GAISLER_AHBSTAT, "GAISLER_AHBSTAT"},
	{GAISLER_DDR2SPA, "GAISLER_DDR2SPA"},
	{GAISLER_DDRSPA, "GAISLER_DDRSPA"},
	{0, NULL}
};

static device_name_s esa_device_name[] = {
	{ESA_LEON2, "ESA_LEON2"},
	{ESA_MCTRL, "ESA_MCTRL"},
	{ESA_SPW2, "ESA_SPW2"},
	{0, NULL}
};

static const char *amba_device2str(unsigned int vendor, unsigned int device)
{
	device_name_s *tab = NULL;
	int i = 0;

	switch (vendor)
	{
	 case VENDOR_GAISLER:
		 tab = gaisler_device_name;
		 break;
	 case VENDOR_ESA:
		 tab = esa_device_name;
		 break;
	 default:
		 return "N/A";
		 break;
	}

	for (i = 0; tab[i].id != 0; i++)
		if (tab[i].id == device)
			return tab[i].name;
	return "N/A";
}

void amba_print(void)
{
	int i = 0;

	for (i = 0; i < g_amba_devs.devnbr; i++)
	{
	unsigned int conf = g_amba_devs.dev[i].addr[0];

		pdebug(DS_AMBA, DL_INFO, "AMBA : %s %x:%x %s irq=%u iobar=0x%x\n\r",
					 amba_type2str(g_amba_devs.dev[i].type),
					 amba_get_vendor(conf),
					 amba_get_device(conf),
					 amba_device2str(amba_get_vendor(conf), amba_get_device(conf)),
					 amba_irq(conf),
					 amba_iobar_start((unsigned int) g_amba_devs.dev[i].masteraddr,
														g_amba_devs.dev[i].addr[1]));
	}
}
