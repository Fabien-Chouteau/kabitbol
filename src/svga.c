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
#include "debug.h"
#include "svga.h"

volatile svga_s *g_svga = NULL;

int svga_init(void)
{
	const amba_dev_s *dev =
		amba_find_device(AMBA_APB_SLAVE, VENDOR_GAISLER, GAISLER_SVGA);

	if (dev == NULL)
		return ERROR;

	g_svga =
		(svga_s *) amba_iobar_start((unsigned int) dev->masteraddr, dev->addr[1]);

	if (g_svga == NULL)
		return ERROR;

	g_svga->fb_pos = 0x40000800;	/* display the kernel */
	g_svga->status = g_svga->status | 1;


	/* set config */

	unsigned int xres = 640;
	unsigned int yres = 480;
	unsigned int pixclock = 40000;
	unsigned int left_margin = 48;
	unsigned int right_margin = 16;
	unsigned int upper_margin = 31;
	unsigned int lower_margin = 11;
	unsigned int hsync_len = 96;
	unsigned int vsync_len = 2;

	g_svga->video_length = ((yres - 1) << 16) + (xres - 1);
	g_svga->front_porch = (lower_margin << 16) + (right_margin);
	g_svga->sync_length = (vsync_len << 16) + (hsync_len);
	g_svga->line_length =
		((yres + lower_margin + upper_margin + vsync_len - 1) << 16) + (xres +
																																		right_margin
																																		+
																																		left_margin
																																		+
																																		hsync_len -
																																		1);

	pdebug(DS_SVGA, DL_INFO, "SVGA : bar = 0x%x\r\n", g_svga);

	int i = 0;
	int clk_sel = -1;
	for (i = 0; i <= 3; i++)
	{
		if (pixclock == g_svga->clk_vector[i])
		{
			clk_sel = i;
		}
	}

	pdebug(DS_SVGA, DL_INFO, "SVGA : clock select = %u\r\n", clk_sel);

	if (clk_sel < 0)
		return ERROR;

	g_svga->status = (clk_sel << 6) | (0x1 /* 8 bits */  << 4) | 1;

	for (i = 0; i < 9999999; i++)
		continue;

	g_svga->status = g_svga->status | 1;
	return NOERROR;
}
