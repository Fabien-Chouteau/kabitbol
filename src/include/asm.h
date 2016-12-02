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

#ifndef ASM_H_
# define ASM_H_

# define trap_handle(trap_nbr)\
  mov %wim, %l0;																												\
  mov %psr, %l3;																												\
  b trap_prehandler;																										\
  mov trap_nbr, %l4;						/* keep the trap number, the PSR and call the trap handler */

# define trap_reset()\
  b reset_prehandler; nop; nop; nop;	/* branch to prehandler */

# define trap_special_handle(handler)\
  b handler; nop; nop; nop;			/* branch to special handler */


# define L0_OFFSET			0x00
# define L1_OFFSET			0x04
# define L2_OFFSET			0x08
# define L3_OFFSET			0x0c
# define L4_OFFSET			0x10
# define L5_OFFSET			0x14
# define L6_OFFSET			0x18
# define L7_OFFSET			0x1c
# define I0_OFFSET			0x20
# define I1_OFFSET			0x24
# define I2_OFFSET			0x28
# define I3_OFFSET			0x2c
# define I4_OFFSET			0x30
# define I5_OFFSET			0x34
# define I6_OFFSET			0x38
# define I7_OFFSET			0x3c

/*
 * others registers are saved "under" the stack 
 * st %xx, [%sp - OFFSET]
*/
# define G7_OFFSET			0x04
# define G6_OFFSET			0x08
# define G5_OFFSET			0x0c
# define G4_OFFSET			0x10
# define G3_OFFSET			0x14
# define G2_OFFSET			0x18
# define G1_OFFSET			0x1c

# define WIM_OFFSET         0x40
# define PSR_OFFSET         0x44
# define Y_OFFSET           0x48
# define PC_OFFSET          0x4c
# define NPC_OFFSET         0x50
# define RESTORE_CNT_OFFSET 0x54


#endif /* ! ASM_H_ */
