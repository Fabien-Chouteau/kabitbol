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

#ifndef MM_H_
# define MM_H_

# include "config.h"

# define MM_ET_INVALID 0x0
# define MM_ET_PTD     0x1
# define MM_ET_PTE     0x2

/* Acces permisions */
# define MM_ACC_R      (0x0 << 2)	/* All Read only */
# define MM_ACC_RW     (0x1 << 2)	/* All Read Write */
# define MM_ACC_RE     (0x2 << 2)	/* All Read Execute */
# define MM_ACC_RWE    (0x3 << 2)	/* All Read Write Execute */
# define MM_ACC_E      (0x4 << 2)	/* All Execute only */
# define MM_ACC_R_S_RW (0x5 << 2)	/* User Read only, Supervisor Read Write */

# define MM_ACC_S_RE   (0x6 << 2)	/* Supervisor Read Execute */
# define MM_ACC_S_RWE  (0x7 << 2)	/* Supervisor Read Write Execute */

# define MM_CACHEABLE  (1 << 7)
# define MM_MODIFIED   (1 << 6)
# define MM_REFERENCED (1 << 5)

# define MM_LVL1_ENTRIES_NBR 256
# define MM_LVL1_PAGE_SIZE (64 * 64 * 4 * 1024)	/* 16 MegaBytes */
# define mm_index1(addr) (((addr) >> 24) & 0xFF)

# define MM_LVL2_ENTRIES_NBR 64
# define MM_LVL2_PAGE_SIZE (64 * 4 * 1024)	/* 256 KiloBytes */
# define mm_index2(addr) (((addr) >> 18) & 0x3F)

# define MM_LVL3_ENTRIES_NBR 64
# define MM_LVL3_PAGE_SIZE (4 * 1024)	/* 4 KiloBytes */
# define mm_index3(addr) (((addr) >> 12) & 0x3F)

# define MM_GET_PT_ADDR(ptd) (void *)(((unsigned int)(ptd) & ~0x3) << 4)

/* MMU ASI and registers */
# define MM_ASI_M_MMUREGS 0x19	/* not sparc v8 compliant */
# define MM_CTRL_REG      0x00000000
# define MM_CTXTBL_PTR    0x00000100
# define MM_CTX_REG       0x00000200
# define MM_FAULT_STATUS  0x00000300
# define MM_FAULT_ADDR    0x00000400

# define MM_KERNEL_LVL1_INDEX_CODE (mm_index1(RAM_ADDR))
# define MM_KERNEL_LVL1_INDEX_HEAP (mm_index1(RAM_ADDR + MM_LVL1_PAGE_SIZE))


# define mm_ctrl_print(section, lvl, mm_ctrl) do {										\
unsigned int _ctrl = (mm_ctrl);																		\
 pdebug(section, lvl, "MMU control register : impl:0x%x ver:%x PSO:%u NF:%u E:%u\n\r", \
        (_ctrl >> 28) & 0xF, (_ctrl >> 24) & 0xF,                       \
        (_ctrl >> 7) & 0x1, (_ctrl >> 1) & 0x1, (_ctrl ) & 0x1);				\
  } while (0)

typedef unsigned int pte;
/* typedef struct	pte */
/* { */
/* 		unsigned int et:2;					/\* Entry type, should be 0x2 for a PTE *\/ */
/* 		unsigned int acc:3;					/\* Access permision *\/ */
/* 		unsigned int r:1;						/\* Referenced *\/ */
/* 		unsigned int m:1;						/\* Modified *\/ */
/* 		unsigned int c:1;						/\* Cacheable *\/ */
/* 		unsigned int ppn:24;				/\* Physical Page Number *\/ */
/* }								__attribute__((packed)) pte_s ; */

typedef unsigned int ptd;
/* typedef struct	ptd */
/* { */
/* 		unsigned int et:2;					/\* Entry type, should be 0x1 for a PTD *\/ */
/* 		unsigned int ppn:30;				/\* Page Table Pointer *\/ */
/* }								__attribute__((packed)) ptd_s; */

extern ptd g_contexts_tab[MAX_CTX_NBR];

int mm_init(void);

int page_fault_handler(void);

#endif /* ! MM_H_ */
