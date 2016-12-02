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

#ifndef AMBA_H_
# define AMBA_H_

/* Vandor and device id from rterms */
/* Vendor codes */
# define VENDOR_GAISLER   1
# define VENDOR_PENDER    2
# define VENDOR_ESA       4
# define VENDOR_OPENCORES 8

/* Gaisler Research device id's */
# define GAISLER_LEON3     0x03
# define GAISLER_LEON3DSU  0x04
# define GAISLER_ETHAHB    0x05
# define GAISLER_APBMST    0x06
# define GAISLER_AHBUART   0x07
# define GAISLER_SRCTRL    0x08
# define GAISLER_SDCTRL    0x09
# define GAISLER_APBUART   0x0C
# define GAISLER_IRQMP     0x0D
# define GAISLER_AHBRAM    0x0E
# define GAISLER_GPTIMER   0x11
# define GAISLER_PCITRG    0x12
# define GAISLER_PCISBRG   0x13
# define GAISLER_PCIFBRG   0x14
# define GAISLER_PCITRACE  0x15
# define GAISLER_DMACTRL   0x16
# define GAISLER_OCCAN     0x19
# define GAISLER_PIOPORT   0x1A
# define GAISLER_ETHMAC    0x1D
# define GAISLER_SPACEWIRE 0x1f
# define GAISLER_AHB2AHB   0x20
# define GAISLER_I2CMST    0x28
# define GAISLER_GRSPW2    0x29
# define GAISLER_GRCAN     0x34
# define GAISLER_GRHCAN    0x3d
# define GAISLER_GRFIFO    0x35
# define GAISLER_GRADCDAC  0x36
# define GAISLER_GRPULSE   0x37
# define GAISLER_GRTIMER   0x38
# define GAISLER_FTAHBRAM  0x50
# define GAISLER_FTMCTRL   0x54
# define GAISLER_BRM       0x72
# define GAISLER_PCIDMA   0x016
# define GAISLER_AHBTRACE 0x017
# define GAISLER_ETHDSU   0x018
# define GAISLER_AHBJTAG  0x01c
# define GAISLER_SPW      0x01f
# define GAISLER_ATACTRL  0x024
# define GAISLER_VGA      0x061
# define GAISLER_SVGA      0x063
# define GAISLER_KBD      0X060
# define GAISLER_DDRSPA   0x025
# define GAISLER_EHCI     0x026
# define GAISLER_UHCI     0x027
# define GAISLER_SPW2     0x029
# define GAISLER_DDR2SPA  0x02E
# define GAISLER_AHBSTAT  0x052
# define GAISLER_L2TIME   0xffd
# define GAISLER_L2C      0xffe
# define GAISLER_PLUGPLAY 0xfff

/* European Space Agency device id's */
# define ESA_LEON2        0x2
# define ESA_MCTRL        0xF
# define ESA_SPW2         0x12

/* Opencores device id's */
# define OPENCORES_PCIBR  0x4
# define OPENCORES_ETHMAC 0x5

/* End Of rtems vendor device ID */


# define AMBA_AHB_MASTERS_NBR 16
# define AMBA_AHB_SLAVES_NBR  16
# define AMBA_APB_SLAVES_NBR  16

# define AMBA_DEVICES_NBR (AMBA_AHB_MASTERS_NBR + AMBA_AHB_SLAVES_NBR + AMBA_APB_SLAVES_NBR)	/* max number of amba devices on the system */

# define AMBA_CONF_AREA 0xff000
# define AMBA_AHB_SLAVE_CONF_AREA (1 << 11)

# define AMBA_AHB_CONF_WORDS 8
# define AMBA_APB_CONF_WORDS 2

# define LEON3_IO_AREA   0xfff00000
# define LEON3_CONF_AREA 0xff000

typedef enum amba_dev_type
{
	AMBA_AHB_MASTER,
	AMBA_AHB_SLAVE,
	AMBA_APB_SLAVE
} amba_dev_type_e;


typedef struct amba_dev
{
	amba_dev_type_e type;
	unsigned int *addr;
	unsigned int *masteraddr;
} amba_dev_s;

typedef struct amba_dev_table
{
	int devnbr;
	amba_dev_s dev[AMBA_DEVICES_NBR];
	unsigned int *ioarea;
} amba_dev_table_s;

# define amba_add_dev(table, _addr, _type, _master)\
	{                                               \
    (table)->dev[(table)->devnbr].type = (_type);               \
    (table)->dev[(table)->devnbr].addr = (unsigned int *)(_addr);       \
    (table)->dev[(table)->devnbr].masteraddr = (unsigned int *)(_master); \
    (table)->devnbr++;                                                  \
  } while (0)

# define amba_get_vendor(x) (((x) >> 24) & 0xff)

# define amba_get_device(x) (((x) >> 12) & 0xfff)

# define amba_membar_start(mbar) (((mbar) & 0xfff00000) & (((mbar) & 0xfff0) << 16))

# define amba_iobar_start(base, iobar) ((base) | ((((iobar) & 0xfff00000)>>12) & (((iobar) & 0xfff0)<<4)) )

# define amba_irq(conf) ((conf) & 0x1f)

int amba_init();
void amba_print(void);

const amba_dev_s *amba_find_device(amba_dev_type_e type, unsigned int vendor,
																	 unsigned int device);

#endif /* ! AMBA_H_ */
