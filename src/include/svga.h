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

#ifndef SVGA_H_
# define SVGA_H_

# define svga_status_EN   (1 << 0)	/* Enable */
# define svga_status_RST  (1 << 1)	/* Reset */
# define svga_status_VR   (1 << 3)	/* Vertical Refresh */
# define svga_status_BDS  (0x3 << 4)	/* Bit depth selector */
# define svga_status_CS   (0x3 << 6)	/* Clock Select */
# define svga_status_HPOL (1 << 8)	/* H polarity */
# define svga_status_VPOL (1 << 9)	/* V polarity */



/* Many informations in snapgear driver and grlib doc */

typedef struct svga
{
	volatile unsigned int status;
	volatile unsigned int video_length;	/* Video Length VRES(31-16) | HRES(15-0) (in pixels - 1) */
	volatile unsigned int front_porch;	/* Front porch VPORCH(31-16) | HPORCH(15-0) (in pixels) */
	volatile unsigned int sync_length;	/* Sync pulse VPLEN(31-16) | HPLEN(15-0) (in pixels - 1) */
	volatile unsigned int line_length;	/* Line Length VLLEN(31-16) | HLLEN(15-0) (in pixels - 1) */
	volatile unsigned int fb_pos;	/* Framebuffer Memory Position must be aligned on a 1 Kbyte boundary. */
	volatile unsigned int clk_vector[4];	/* Dynamic clock defined in ps */
	volatile unsigned int clut;	/* CLUT Access
															 * 31:24 Color lookup table register (CREG) - Color lookup table register to set.
															 * 23:16 Red color data (RED) - Red color data to set in the specified register.
															 * 15:8  Green color data (GREEN) - Green color data to set in the specified register.
															 * 7:0   Blue color data (BLUE) - Blue color data to set in the specified register.
															 */
} svga_s;

# define svga_color_reg(red, green, blue) ((red << 16) | (green << 8) | blue)

# define svga_data_reg(addr, data) (((addr) << 8) | ((data) & 0xFF))

# define svga_BLACK vga_color_reg(0, 0, 0)
# define svga_WHITE vga_color_reg(255, 255, 255)

# define svga_RED   vga_color_reg(255, 0, 0)
# define svga_GREEN vga_color_reg(0, 255, 0)
# define svga_BLUE  vga_color_reg(0, 0, 255)

int svga_init(void);

#endif /* ! SVGA_H_ */
