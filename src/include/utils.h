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

#ifndef UTILS_H_
# define UTILS_H_

# include "core.h"

void memset(unsigned char *buf, unsigned char val, unsigned int size);

int printf(const char *format, ...);

static inline void asi_set(unsigned int addr, unsigned int value,
													 unsigned char asi)
{
	asm volatile ("sta %0, [%1] 0x19;\n"::"r" (value), "r"(addr):"memory");
}

static inline unsigned int asi_get(unsigned int addr, unsigned char asi)
{
	unsigned int value = 0;
	asm volatile ("lda [%1] 0x19, %0;\n":"=r" (value):"r"(addr):"memory");
	return value;
}

# define ASI_MMU_BYPASS 0x1c		/* not sparc v8 compliant */

static inline void outb(uint32_t addr, uint8_t data)
{
  asm volatile ("sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (data), "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
}

static inline void outw(uint32_t addr, uint32_t data)
{
  asm volatile ("sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (data), "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
}

static inline uint8_t inb(uint32_t addr)
{
  uint8_t value = 0;
  asm volatile ("lduba [%1] %2, %0;\n"
                : "=r" (value)
                : "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
  return value;
}

static inline uint32_t inw(uint32_t addr)
{
  uint32_t value = 0;
  asm volatile ("lda [%1] %2, %0;\n"
                : "=r" (value)
                : "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
  return value;
}
#endif /* ! UTILS_H_ */
