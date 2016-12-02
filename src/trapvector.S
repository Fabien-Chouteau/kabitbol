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

#include "asm.h"
#include "config.h"

# define PSR_ET					0x20	/* enable traps */
# define PSR_PS					0x40	/* previous supervisor */
# define PSR_S					0x80	/* supervisor */
# define PSR_CWP_MASK		0x1F	/* Current Window Pointer */

.section .trapvector
.align 4
.global _start
.global trapvector
_start:
trapvector:
	trap_reset()			// reset
	trap_handle(0x01)	// inst_access_exception
	trap_handle(0x02)	// illegal_instruction
	trap_handle(0x03)	// privileged_instruction
	trap_handle(0x04)	// fp_disabled
	trap_special_handle(window_overflow_trap_handler)	 // 0x05 window_overflow
	trap_special_handle(window_underflow_trap_handler) // 0x06 window_underflow
	trap_handle(0x07)	// mem_address_not_aligned
	trap_handle(0x08)	// fp_exception
	trap_handle(0x09)	// data_access_exception
	trap_handle(0x0A)	// tag_overflow
	trap_handle(0x0B)	// watchpoint_detected

	trap_handle(0x0C) trap_handle(0x0D) // unhandled
	trap_handle(0x0E)	trap_handle(0x0F) trap_handle(0x10)

	trap_handle(0x11) // IRQs
	trap_handle(0x12)
	trap_handle(0x13)
	trap_handle(0x14)
	trap_handle(0x15)
	trap_handle(0x16)
	trap_handle(0x17)
	trap_handle(0x18)
	trap_handle(0x19)
	trap_handle(0x1A)
	trap_handle(0x1B)
	trap_handle(0x1C)
	trap_handle(0x1D)
	trap_handle(0x1E)


	trap_handle(0x1f)
	trap_handle(0x20)	// r_register_access_error
	trap_handle(0x21)	// inst_access_error
	trap_handle(0x22) trap_handle(0x23)
	trap_handle(0x24)	// cp_disabled
	trap_handle(0x25)	// unimplemented_FLUSH
	trap_handle(0x26) trap_handle(0x27)
	trap_handle(0x28)	// cp_exception
	trap_handle(0x29)	// data_access_error
	trap_handle(0x2A)	// division_by_zero
	trap_handle(0x2B)	// data_store_error 
	trap_handle(0x2C)	// data_access_MMU_miss
	trap_handle(0x2d)
	trap_handle(0x2e) trap_handle(0x2f) trap_handle(0x30)
	trap_handle(0x31) trap_handle(0x32) trap_handle(0x33)
	trap_handle(0x34) trap_handle(0x35) trap_handle(0x36)
	trap_handle(0x37) trap_handle(0x38) trap_handle(0x39)
	trap_handle(0x3a) trap_handle(0x3b)
	trap_handle(0x3C)	// inst_access_MMU_miss
	trap_handle(0x3d) trap_handle(0x3e) trap_handle(0x3f)
	trap_handle(0x40) trap_handle(0x41) trap_handle(0x42)
	trap_handle(0x43) trap_handle(0x44) trap_handle(0x45)
	trap_handle(0x46) trap_handle(0x47) trap_handle(0x48)
	trap_handle(0x49) trap_handle(0x4a) trap_handle(0x4b)
	trap_handle(0x4c) trap_handle(0x4d) trap_handle(0x4e)
	trap_handle(0x4f) trap_handle(0x50) trap_handle(0x51)
	trap_handle(0x52) trap_handle(0x53) trap_handle(0x54)
	trap_handle(0x55) trap_handle(0x56) trap_handle(0x57)
	trap_handle(0x58) trap_handle(0x59) trap_handle(0x5a)
	trap_handle(0x5b) trap_handle(0x5c) trap_handle(0x5d)
	trap_handle(0x5e) trap_handle(0x5f) trap_handle(0x60)
	trap_handle(0x61) trap_handle(0x62) trap_handle(0x63)
	trap_handle(0x64) trap_handle(0x65) trap_handle(0x66)
	trap_handle(0x67) trap_handle(0x68) trap_handle(0x69)
	trap_handle(0x6a) trap_handle(0x6b) trap_handle(0x6c)
	trap_handle(0x6d) trap_handle(0x6e) trap_handle(0x6f)
	trap_handle(0x70) trap_handle(0x71) trap_handle(0x72)
	trap_handle(0x73) trap_handle(0x74) trap_handle(0x75)
	trap_handle(0x76) trap_handle(0x77) trap_handle(0x78)
	trap_handle(0x79) trap_handle(0x7a) trap_handle(0x7b)
	trap_handle(0x7c) trap_handle(0x7d) trap_handle(0x7e)
	trap_handle(0x7f) trap_handle(0x80) trap_handle(0x81)
	trap_handle(0x82) trap_handle(0x83) trap_handle(0x84)
	trap_handle(0x85) trap_handle(0x86) trap_handle(0x87)
	trap_handle(0x88) trap_handle(0x89) trap_handle(0x8a)
	trap_handle(0x8b) trap_handle(0x8c) trap_handle(0x8d)
	trap_handle(0x8e) trap_handle(0x8f) trap_handle(0x90)
	trap_handle(0x91) trap_handle(0x92) trap_handle(0x93)
	trap_handle(0x94) trap_handle(0x95) trap_handle(0x96)
	trap_handle(0x97) trap_handle(0x98) trap_handle(0x99)
	trap_handle(0x9a) trap_handle(0x9b) trap_handle(0x9c)
	trap_handle(0x9d) trap_handle(0x9e) trap_handle(0x9f)
	trap_handle(0xa0) trap_handle(0xa1) trap_handle(0xa2)
	trap_handle(0xa3) trap_handle(0xa4) trap_handle(0xa5)
	trap_handle(0xa6) trap_handle(0xa7) trap_handle(0xa8)
	trap_handle(0xa9) trap_handle(0xaa) trap_handle(0xab)
	trap_handle(0xac) trap_handle(0xad) trap_handle(0xae)
	trap_handle(0xaf) trap_handle(0xb0) trap_handle(0xb1)
	trap_handle(0xb2) trap_handle(0xb3) trap_handle(0xb4)
	trap_handle(0xb5) trap_handle(0xb6) trap_handle(0xb7)
	trap_handle(0xb8) trap_handle(0xb9) trap_handle(0xba)
	trap_handle(0xbb) trap_handle(0xbc) trap_handle(0xbd)
	trap_handle(0xbe) trap_handle(0xbf) trap_handle(0xc0)
	trap_handle(0xc1) trap_handle(0xc2) trap_handle(0xc3)
	trap_handle(0xc4) trap_handle(0xc5) trap_handle(0xc6)
	trap_handle(0xc7) trap_handle(0xc8) trap_handle(0xc9)
	trap_handle(0xca) trap_handle(0xcb) trap_handle(0xcc)
	trap_handle(0xcd) trap_handle(0xce) trap_handle(0xcf)
	trap_handle(0xd0) trap_handle(0xd1) trap_handle(0xd2)
	trap_handle(0xd3) trap_handle(0xd4) trap_handle(0xd5)
	trap_handle(0xd6) trap_handle(0xd7) trap_handle(0xd8)
	trap_handle(0xd9) trap_handle(0xda) trap_handle(0xdb)
	trap_handle(0xdc) trap_handle(0xdd) trap_handle(0xde)
	trap_handle(0xdf) trap_handle(0xe0) trap_handle(0xe1)
	trap_handle(0xe2) trap_handle(0xe3) trap_handle(0xe4)
	trap_handle(0xe5) trap_handle(0xe6) trap_handle(0xe7)
	trap_handle(0xe8) trap_handle(0xe9) trap_handle(0xea)
	trap_handle(0xeb) trap_handle(0xec) trap_handle(0xed)
	trap_handle(0xee) trap_handle(0xef) trap_handle(0xf0)
	trap_handle(0xf1) trap_handle(0xf2) trap_handle(0xf3)
	trap_handle(0xf4) trap_handle(0xf5) trap_handle(0xf6)
	trap_handle(0xf7) trap_handle(0xf8) trap_handle(0xf9)
	trap_handle(0xfa) trap_handle(0xfb) trap_handle(0xfc)
	trap_handle(0xfd) trap_handle(0xfe) trap_handle(0xff)

.global trap_prehandler
.text
trap_prehandler:
 /*
  * %l0 = %wim
  *   %l1 = %pc
  *   %l2 = %npc
  *   %l3 = %psr
  *   %l4 = trap_nbr
  */
  sub   %fp, 0x40, %sp

  rd    %psr, %l6
  or    %l6, 0xFE0, %l5 // disable interrupts enable traps
  wr    %l5, %psr
  nop
  nop
  nop

  call  context_save
  nop // delay slot
  /* %g6 = restore counter from context_save */
  mov   %fp, %l6

  /* return on kernel stack */

  set   _kstack, %l7
  sub   %l7, 0x40, %l7
  andn  %l7, 0x0f, %l7    /* align stack on 16-byte boundary */

  mov   %l7, %sp  // Set Stack pointer
  mov   %l7, %fp  // Set Frame pointer

  mov   %l1, %o0  // %pc - 1st arg
  mov   %l2, %o1  // %npc - 2nd arg
  mov   %l3, %o2  // %psr - 3rd arg
  mov   %l4, %o3  // trap_nbr - 4th arg
  mov   %g6, %o4  // restore counter - 5th arg
  call  trap_handler
  mov   %l6, %o5  // stack pointer - 6th arg

  sethi %hi(g_restore_sp),%g6
  or    %g6, %lo(g_restore_sp), %g6
  ld    [%g6], %g2

  call  context_restore
  nop
 /*
  *   After context_restore
  *   %l2 = sp
  *   %l3 = pc
  *   %l4 = npc
  */

  /* Disable Traps, enable interrupts and clear PS before rett */
  mov   %psr, %l0
  andn  %l0, 0xF60, %l0
  mov   %l0, %psr
  nop
  nop
  nop

	/* RTFM */
  jmpl  %l3, %g0  // pc
  rett  %l4       // npc


.global context_save
reset_prehandler:
	flush
	set		PSR_INIT, %g1		! init IU
	mov		%g1, %psr
	nop
	nop
	nop

	mov		0x2, %wim
	mov		%g0, %tbr
	mov		%g0, %y
	mov		%g0, %asr16
	nop

	// set TBR : Trap base register
	set			trapvector, %g1
	mov			%g1, %tbr


	// Read cpu id
  rd		%asr17, %g3
  srl		%g3, 28, %g3
	umul	%g3, 200, %g3 // shift stack for each cpu

	// set SP : Stack pointer and FP : Frame pointer
	sethi		%hi(_kstack),%l1
	or    	%l1, %lo(_kstack), %l1
	sub			%l1, %g3,  %l1	// one stack for each cpu
	sub			%l1, 0x40, %l1	// stack starts at end of PARTITION - 0x40
	andn		%l1, 0x0f, %l1	// align stack on 16-byte boundary
	mov			%l1, %sp				// Set Stack pointer
	mov			%l1, %fp				// Set Frame pointer

	call  kabitbol_init
	nop

	deadloop:
	b deadloop
	nop
	set			trapvector, %g1
	rett		%g1 /* unreachable code -> reset */
	nop

.global context_save
context_save:
        /*
        * input:
        *   %l0 = %wim
        *   %l1 = %pc
        *   %l2 = %npc
        *   %l3 = %psr
        *   %l4 = trap_nbr
        * output:
        *   %l5 = restore counter
        *
        *
        *   little reminder of the register windows concept
        *   CWP = 4
        *   WIM = 0b00000010
        *
        *   Windows:
        *   0 - used
        *   1 - invalid
        *   2 - unused
        *   3 - unused
        *   4 - unused
        *   5 - current
        *   6 - used
        *   7 - used
        *
        *   The 5th window is the trap window.
        *   We have to save all the used windows.
        *   With the "restore" instruction we can
        *   walk through windows until we reach the invalid one.
        */

        /* Disable Traps */
        mov   %psr, %o0
        andn  %o0, PSR_ET, %o0
        mov   %o0, %psr
        nop
        nop
        nop

        /* save the global and input registers "under" the stack */
        st    %g1, [%fp - G1_OFFSET]
        st    %g2, [%fp - G2_OFFSET]
        st    %g3, [%fp - G3_OFFSET]
        st    %g4, [%fp - G4_OFFSET]
        st    %g5, [%fp - G5_OFFSET]
        st    %g6, [%fp - G6_OFFSET]
        st    %g7, [%fp - G7_OFFSET]
        st    %i0, [%fp - I0_OFFSET]
        st    %i1, [%fp - I1_OFFSET]
        st    %i2, [%fp - I2_OFFSET]
        st    %i3, [%fp - I3_OFFSET]
        st    %i4, [%fp - I4_OFFSET]
        st    %i5, [%fp - I5_OFFSET]
        st    %i6, [%fp - I6_OFFSET]
        st    %i7, [%fp - I7_OFFSET]

        and   %o0, PSR_CWP_MASK, %g3 // %g3 = CWP
        mov   1, %g2
        sll   %g2, %g3, %g2 // %g2 = Windows current mask
        mov   %wim, %g1
        mov   %o7, %g7
        clr   %g6 // restore counter
save_loop:
       /*  The loop with our example.
        *   00000010 : %g1 WIM
        *   00100000 : %g2 init
        *   01000000 : %g2 1st loop : ok
        *   10000000 : %g2 2nd loop : ok
        *   00000001 : %g2 3rd loop : ok
        *   00000010 : %g2 4th loop : ((%g2 & %g1) != 0) => quit
        */
        sll   %g2, 1, %g4
        srl   %g2, WINDOWS_NBR - 1, %g2
        or    %g2, %g4, %g2 // nice shift left with modulo :)

        andcc %g2, %g1, %g0
        bnz   end_of_save_loop
        nop

        /* We still have some register set to save */
        restore

        inc   %g6
        /* little optimisation with store double */
        std   %l0, [%sp + L0_OFFSET]
        std   %l2, [%sp + L2_OFFSET]
        std   %l4, [%sp + L4_OFFSET]
        std   %l6, [%sp + L6_OFFSET]

        std   %i0, [%sp + I0_OFFSET]
        std   %i2, [%sp + I2_OFFSET]
        std   %i4, [%sp + I4_OFFSET]

        ba    save_loop
        std   %i6, [%sp + I6_OFFSET]  // delay slot

end_of_save_loop:

        /* return in the trap window */
        mov   %psr, %g5
        andn  %g5, PSR_CWP_MASK, %g5  // clear the CWP
        or    %g5, %g3, %g5           // set the CWP saved in %g3 before loop
        or    %g5, PSR_ET, %g5        // enable traps
        mov   %g5, %psr
        nop
        nop
        nop

        /* set new WIM so the trap handler get the maximum number of windows */
        mov   1, %g5
        sll   %g5, %g3, %g5

        sll   %g5, 1, %g1
        srl   %g5, WINDOWS_NBR - 1, %g5
        or    %g5, %g1, %g5
        mov   %g5, %wim

        /* save the others registers "under" the stack */
        st    %l0, [%fp - WIM_OFFSET]
        st    %l3, [%fp - PSR_OFFSET]
        mov   %y, %o3
        st    %o3, [%fp - Y_OFFSET]
        st    %l1, [%fp - PC_OFFSET]
        st    %l2, [%fp - NPC_OFFSET]
        st    %g6, [%fp - RESTORE_CNT_OFFSET]

        jmpl  %g7 + 8, %g0
        nop   // delay slot

.global context_restore
context_restore:
        /*
        *   %g1 = restore counter
        *   %g2 = sp (mov %g2, %fp)
        *   %g3 = pc
        *   %g4 = npc
        *
        *   The goal is to restore the register windows exactly
        *   like there was before the trap.
        *   So we reload the same number of windows.
        *
        *   Example with restore_counter = 4
        *   Windows after restore:
        *   0 - used
        *   1 - used
        *   2 - used
        *   3 - used
        *   4 - invalid
        *   5 - unused
        *   6 - unused
        *   7 - current
        *
        */

        mov   %o7, %g7 // save return addr

        /* Disable Traps and set CWP */
        mov   %psr, %l0

        //andn  %l0, 0x3F, %l0
        andn  %l0, 0xF, %l0

        or    %l0, WINDOWS_NBR - 1, %l0 // CWP = 7
        mov   %l0, %psr
        nop
        nop
        nop

        mov   %g2, %fp
        ld    [%g2 - RESTORE_CNT_OFFSET], %g1
        ld    [%g2 - PC_OFFSET], %g3
        ld    [%g2 - NPC_OFFSET], %g4

        mov   1, %l0
        sll   %l0, %g1 ,%l0
        mov   %l0, %wim

        tst   %g1
        bz    end_of_restore_loop
        nop   // delay slot

restore_loop:

        restore

        /* little optimisation with load double */
        ldd   [%sp + L0_OFFSET], %l0
        ldd   [%sp + L2_OFFSET], %l2
        ldd   [%sp + L4_OFFSET], %l4
        ldd   [%sp + L6_OFFSET], %l6

        ldd   [%sp + I0_OFFSET], %i0
        ldd   [%sp + I2_OFFSET], %i2
        ldd   [%sp + I4_OFFSET], %i4
        ldd   [%sp + I6_OFFSET], %i6

        subcc %g1, 1, %g1
        bnz   restore_loop
        nop   // delay slot


end_of_restore_loop:
        mov   %psr, %g6
        andn  %g6, PSR_CWP_MASK, %g6
        or    %g6, WINDOWS_NBR - 1, %g6 // CWP = 7
        mov   %g6, %psr
        nop
        nop
        nop

        /* save my global registers */
        mov   %g2, %l2  // sp
        mov   %g3, %l3  // pc
        mov   %g4, %l4  // npc
        mov   %g7, %l7  // return addr
        /* restore the context's global and input registers */
        ld    [%l2 - G1_OFFSET], %g1
        ld    [%l2 - G2_OFFSET], %g2
        ld    [%l2 - G3_OFFSET], %g3
        ld    [%l2 - G4_OFFSET], %g4
        ld    [%l2 - G5_OFFSET], %g5
        ld    [%l2 - G6_OFFSET], %g6
        ld    [%l2 - G7_OFFSET], %g7
        ld    [%l2 - I0_OFFSET], %i0
        ld    [%l2 - I1_OFFSET], %i1
        ld    [%l2 - I2_OFFSET], %i2
        ld    [%l2 - I3_OFFSET], %i3
        ld    [%l2 - I4_OFFSET], %i4
        ld    [%l2 - I5_OFFSET], %i5
        ld    [%l2 - I6_OFFSET], %i6
        ld    [%l2 - I7_OFFSET], %i7

        mov   %l2, %fp
        jmpl  %l7 + 8, %g0
        nop   // delay slot

.global start_first_partition
start_first_partition:
	mov		%o0, %g1	// pc
	mov		%o1, %g2	// npc
	mov		%o2, %g3	// sp

	mov		%g3, %fp
	mov		%g3, %sp
	mov		0x4, %wim

	/* Disable Traps and set CWP */
	mov		%psr, %l0
	andn	%l0, 0x7F, %l0 //disable traps and unset PS to switch userland
	mov		%l0, %psr
	nop
	nop
	nop

	/* RTFM */
	jmpl %g1, %g0 // pc
	rett %g2			// npc

window_overflow_trap_handler:

	/*
	*  Calculate new WIM by "rotating" the valid bits in the WIM right
	*  by one position.  The following shows how the bits move for a SPARC
	*  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
	*
	*    OLD WIM = 76543210
	*    NEW WIM = 07654321
	*
	*  NOTE: New WIM must be stored in a global register since the
	*        "save" instruction just prior to the load of the wim 
	*        register will result in the local register set changing.
	*/

	sethi %hi(g_window_overflow),%l4
	or    %l4, %lo(g_window_overflow), %l4
	ld   [%l4], %l3
	inc  %l3
	st   %l3, [%l4]

	mov  %wim, %l3                   ! l3 = WIM
	mov  %g1, %l7                    ! save g1
	srl  %l3, 1, %g1                 ! g1 = WIM >> 1
	sll  %l3, WINDOWS_NBR-1 , %l4
	! l4 = WIM << (Number Windows - 1)
	or   %l4, %g1, %g1               ! g1 = (WIM >> 1) | 
	!      (WIM << (Number Windows - 1))

	save                             ! Get into window to be saved.
	mov  %g1, %wim                   ! load new WIM
	nop; nop; nop                    ! 3 slot delay
	std  %l0, [%sp + 0x00]           ! save local register set
	std  %l2, [%sp + 0x08]
	std  %l4, [%sp + 0x10]
	std  %l6, [%sp + 0x18]
	std  %i0, [%sp + 0x20]           ! save input register set
	std  %i2, [%sp + 0x28]
	std  %i4, [%sp + 0x30]
	std  %i6, [%sp + 0x38]
	restore                          ! Go back to trap window.
	mov  %l7, %g1                    ! restore g1
	jmp  %l1                         ! Re-execute save.
	rett %l2

	/*
	*  Window underflow trap handler.
	*
	*  On entry:
	*
	*    l0 = psr (from trap table)
	*    l1 = pc
	*    l2 = npc
	*/

window_underflow_trap_handler:

	/*
	*  Calculate new WIM by "rotating" the valid bits in the WIM left 
	*  by one position.  The following shows how the bits move for a SPARC
	*  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
	*
	*    OLD WIM = 76543210
	*    NEW WIM = 07654321
	*
	*  NOTE: New WIM must be stored in a global register since the
	*        "save" instruction just prior to the load of the wim 
	*        register will result in the local register set changing.
	*/
	sethi %hi(g_window_underflow),%l4
	or    %l4, %lo(g_window_underflow), %l4
	ld   [%l4], %l3
	inc  %l3
	st   %l3, [%l4]

	mov  %wim, %l3                  ! Calculate new WIM
	sll  %l3, 1, %l4                ! l4 = WIM << 1
	srl  %l3, WINDOWS_NBR-1, %l5
	! l5 = WIM >> (Number Windows-1)
	or   %l5, %l4, %l5              ! l5 = (WIM << 1) |
	!      (WIM >> (Number Windows-1))
	mov  %l5, %wim                  ! load the new WIM
	nop; nop; nop
	restore                         ! Two restores to get into the
	restore                         ! window to restore
	ldd  [%sp + 0x00], %l0          ! First the local register set
	ldd  [%sp + 0x08], %l2
	ldd  [%sp + 0x10], %l4
	ldd  [%sp + 0x18], %l6
	ldd  [%sp + 0x20], %i0          ! Then the input registers
	ldd  [%sp + 0x28], %i2
	ldd  [%sp + 0x30], %i4
	ldd  [%sp + 0x38], %i6
	save                            ! Get back to the trap window.
	save
	jmp  %l1                        ! Re-execute restore.
	rett  %l2

