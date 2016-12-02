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

#ifndef PSR_H_
# define PSR_H_

# define PSR_ET         0x20		/* enable traps */
# define PSR_PS         0x40		/* previous supervisor */
# define PSR_S          0x80		/* supervisor */
# define PSR_CWP_MASK   0x1F		/* Current Window Pointer */
# define PSR_PIL(pil)   (((pil) & 0xF) << 8)	/* PIL */

# define psr_print(section, lvl, psr) do {															\
    unsigned int _psr = (psr);																					\
    pdebug(section, lvl, "%%psr : impl:0x%x ver:%x nzvc:%u%u%u%u EC:%u EF:%u PIL:0x%x S:%u PS:%u ET:%u CWP:%u\n\r", \
           (_psr >> 28) & 0xF, (_psr >> 24) & 0xF,                      \
           (_psr >> 23) & 0x1, (_psr >> 22) & 0x1, (_psr >> 21) & 0x1, (_psr >> 20) & 0x1, \
           (_psr >> 23) & 0x1, (_psr >> 12) & 0x1, (_psr >> 8) & 0xF, (_psr >> 7) & 0x1, (_psr >> 6) & 0x1, \
           (_psr >> 5) & 0x1, _psr & 0xF);                              \
  } while (0)

static inline unsigned int psr_get(void)
{
	unsigned int psr;
	asm volatile ("rd	%%psr, %0\n" "nop\n" "nop\n" "nop\n":"=r" (psr):	/* no inputs */
								:"memory");

	return psr;
}

static inline void psr_set(unsigned int new_psr)
{
	asm volatile ("wr	%0, 0x0, %%psr\n" "nop\n" "nop\n" "nop\n":	/* no outputs */
								:"r" (new_psr):"memory", "cc");
}

static inline void psr_enable_traps(void)
{
	unsigned int psr = psr_get();
	psr |= PSR_ET;
	psr_set(psr);
}

static inline void psr_disable_traps(void)
{
	unsigned int psr = psr_get();
	psr &= ~PSR_ET;
	psr_set(psr);
}

static inline void psr_disable_interupt(void)
{
	unsigned int psr = psr_get();

	psr &= ~(0xF << 8);
	psr_set(psr | PSR_PIL(0xF));
}

static inline void psr_enable_interupt(void)
{
	unsigned int psr = psr_get();

	psr &= ~(0xF << 8);
	psr_set(psr | PSR_PIL(0x0));
}

#endif /* ! PSR_H_ */
