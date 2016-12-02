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
 * Procedure to perform a 32-bit by 32-bit multiply.
 * Pass the multiplicand in %o0, and the multiplier in %o1.
 * The least significant 32 bits of the result are returned in %o0,
 * and the most significant in %o1.
 *
 * This code has an optimization built-in for short (less than 13-bit)
 * multiplies. Short multiplies require 26 or 27 instruction cycles, and
 * long ones require 47 to 51 instruction cycles. For two nonnegative numbers
 * (the most common case) a long multiply takes 47 instruction cycles.
 *
 * This code indicates that overflow has occurred by clearing the Z condition
 * code upon return [note that this is different from condition codes set
 * by the SMULcc and UMULcc instructions]. The following call sequence
 * would be used if one wished to deal with overflow (and did not require
 * the calling software to be SPARC ABI-compliant):
 *
 *          call      .mul
 *          nop                      ! (or set up last parameter here)
 *          bnz       overflow_code ! (or tnz to overflow handler)
 *
 * Note that this is a leaf routine; i.e. it calls no other routines and does
 * all of its work in the out registers. Thus, the usual SAVE and RESTORE
 * instructions are not needed.
 */
      .global .mul
.mul:
      mov        %o0, %y        ! multiplier to Y register
      andncc     %o0, 0xfff, %g0! mask out lower 12 bits
      be         mul_shortway   ! can do it the short way
      andcc      %g0, %g0, %o4 !  zero the partial product and clear N and V conditions
      !
      ! long multiply
      !
      mulscc     %o4, %o1, %o4 !  first iteration of 33
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
     !
     ! If %o0 (multiplier) was negative, the result is:
     !     (%o0 * %o1) + %o1 * (2**32)
     ! We fix that here.
     !
     tst        %o0
     rd         %y, %o0
     bge        1f
     tst        %o0            ! for when we check for overflow
     sub        %o4, %o1, %o4  ! bit 33 and up of the product are in
                               ! %o4, so we don't have to shift %o1
     !
     ! We haven't overflowed if:
     !     low-order bits are nonnegative and high-order bits are 0
     !     low-order bits are negative    and high-order bits are all 1
     !
     ! If you are not interested in detecting overflow,
     ! replace the four following instructions (bge, addcc, retl, subcc) with:
     !          1:   retl
     !               mov       %o4, %o1
     !
1:
     bge        2f             ! if low-order bits were nonnegative.
     addcc      %o4, %g0, %o1  ! return most sig. bits of prod and set
                               ! Z appropriately (for nonnegative product)
     retl                      ! leaf-routine return
     subcc      %o4, -1, %g0   ! set Z if high order bits are -1 (for negative product)
2:
     retl                      ! leaf-routine return
     nop
     !
     ! short multiply
     !
mul_shortway:
     mulscc     %o4, %o1, %o4  ! first iteration of 13
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
   mulscc     %o4, %o1, %o4  ! 12th iteration
   mulscc     %o4, %g0, %o4  ! last iteration only shifts
   rd         %y, %o5
   sll        %o4, 12, %o0   ! left shift middle bits by 12 bits
   srl        %o5, 20, %o5   ! right shift low bits by 20 bits
   !
   ! We haven't overflowed if:
   !     low-order bits are nonnegative and high-order bits are 0
   !     low-order bits are negative    and high-order bits are -1
   !
   ! if you are not interested in detecting overflow,
   ! replace the following code with:
   !
   !          or        %o5, %o0, %o0
   !          retl
   !          mov       %o4, %o1
   !
   orcc       %o5, %o0, %o0 ! merge for true product
   bge        3f             ! if low-order bits were nonnegative.
   sra        %o4, 20, %o1   ! right shift high bits by 20 bits
                             ! and put into %o1
   retl                      ! leaf-routine return
   subcc      %o1, -1, %g0   ! set Z if high order bits are -1 (for
                             ! negative product)
3:
   retl                      ! leaf-routine return
   addcc      %o1, %g0, %g0  ! set Z if high order bits are 0
