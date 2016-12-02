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

/*
 * Procedure to perform a 32 by 32 unsigned multiply.
 * Pass the multiplier in %o0, and the multiplicand in %o1.
 * The least significant 32 bits of the result will be returned in %o0,
 * and the most significant in %o1.
 *
 * This code has an optimization built-in for short (less than 13 bit)
 * multiplies. Short multiplies require 25 instruction cycles, and long ones
 * require 46 or 48 instruction cycles.
 *
 * This code indicates that overflow has occurred, by leaving the Z condition
 * code clear. The following call sequence would be used if you wish to
 * deal with overflow:
 *
 *         call      .umul
 *         nop                      ! (or set up last parameter here)
 *         bnz       overflow_code ! (or tnz to overflow handler)
 *
 * Note that this is a leaf routine; i.e. it calls no other routines and does
 * all of its work in the out registers. Thus, the usual SAVE and RESTORE
 * instructions are not needed.
 */
     .global    .umul
.umul:
     or         %o0, %o1, %o4 ! logical or of multiplier and multiplicand
     mov        %o0, %y        ! multiplier to Y register
     andncc     %o4, 0xfff, %o5! mask out lower 12 bits
     be         mul_shortway   ! can do it the short way
     andcc      %g0, %g0, %o4 ! zero the partial product and clear N and V conditions
     !
     ! long multiply
     !
     mulscc     %o4, %o1, %o4 ! first iteration of 33
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4
     mulscc     %o4, %o1, %o4 ! 32nd iteration
     mulscc     %o4, %g0, %o4 ! last iteration only shifts
     /*
      * Normally, with the shift and add approach, if both numbers are
      * nonnegative, you get the correct result. With 32-bit twos-complement
      * numbers, -x can be represented as ((2 - (x/(2**32))) mod 2) * 2**32.
      * To avoid a lot of 2**32's, we can just move the radix point up to be
      * just to the left of the sign bit. So:
      *
      *     x * y    = (xy) mod 2
      *    -x * y    = (2 - x) mod 2 * y = (2y - xy) mod 2
      *     x * -y   = x * (2 - y) mod 2 = (2x - xy) mod 2
      *    -x * -y   = (2 - x) * (2 - y) = (4 - 2x - 2y + xy) mod 2
      *
      * For signed multiplies, we subtract (2**32) * x from the partial
      * product to fix this problem for negative multipliers (see .mul in
      * Section 1.
      * Because of the way the shift into the partial product is calculated
      * (N xor V), this term is automatically removed for the multiplicand,
      * so we don't have to adjust.
      *
      * But for unsigned multiplies, the high order bit wasn't a sign bit,
      * and the correction is wrong. So for unsigned multiplies where the
      * high order bit is one, we end up with xy - (2**32) * y. To fix it
      * we add y * (2**32).
      */
     tst        %o1
     bge        1f
     nop
     add        %o4, %o0, %o4
1:
     rd         %y, %o0        !  return least sig. bits of prod
     retl                      !  leaf-routine return
     addcc      %o4, %g0, %o1  !  delay slot; return high bits and set
                               !  zero bit appropriately
     !
     ! short multiply
     !
mul_shortway:
     mulscc     %o4, %o1, %o4  ! first iteration of 13
     mulscc     %o4, %o1, %o4
		 mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
		 mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4
     mulscc    %o4, %o1, %o4  ! 12th iteration
     mulscc    %o4, %g0, %o4  ! last iteration only shifts
     rd        %y, %o5
     sll       %o4, 12, %o4   ! left shift partial product by 12 bits
     srl       %o5, 20, %o5   ! right shift product by 20 bits
     or        %o5, %o4, %o0 ! merge for true product
!
! The delay instruction (addcc) moves zero into %o1,
! sets the zero condition code, and clears the other conditions.
! This is the equivalent result to a long umultiply which doesn't overflow.
!
     retl                     ! leaf-routine return
     addcc     %g0, %g0, %o1
