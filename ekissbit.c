/* ekissbit.c - ekiss random bit generator Version 1.0.0             */
/* Copyright (C) 2016 aquila62 at github.com                         */

/* This program is free software; you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License as    */
/* published by the Free Software Foundation; either version 2 of    */
/* the License, or (at your option) any later version.               */

/* This program is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of    */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the      */
/* GNU General Public License for more details.                      */

/* You should have received a copy of the GNU General Public License */
/* along with this program; if not, write to:                        */

   /* Free Software Foundation, Inc.                                 */
   /* 59 Temple Place - Suite 330                                    */
   /* Boston, MA 02111-1307, USA.                                    */

/********************************************************/
/* Thanks to Dr. Robert G. Brown at Duke University     */
/* for many suggestions leading to this RNG.            */
/********************************************************/

#include "ekiss.h"

int ekissbit(kissfmt *kk)
   {
   int i;
   int bit;
   unsigned int *p;           /* state pointer */
   unsigned int tmp;          /* used for bays-durham shuffle */
   /***********************************************************/
   /* back up the previous two states                         */
   /***********************************************************/
   kk->ofst  = kk->pprev >> 22;   /* offset into state array  */
   kk->pprev = kk->prev;   /* prev ==> prev prev              */
   kk->prev  = kk->out;    /* current ==> prev                */
   /***********************************************************/
   /* Calculate the kiss algorithm inline to save a call      */
   /* to ekiss()                                              */
   /***********************************************************/
   /* The KISS macro is in ekiss.h                            */
   /* XOR the previous two results with the current output    */
   kk->out = (KISS ^ kk->pprev ^ kk->prev);
   /***********************************************************/
   /* Bays-Durham shuffle of state array                      */
   /* 1024! = 5.41e+2639 base 10 rounded down                 */
   /* The period length is approximately the factorial of     */
   /* 1024.                                                   */
   /***********************************************************/
   /* point into the state array */
   p         = (unsigned int *) kk->state + kk->ofst;
   /* swap the current output with a member in the state array */
   tmp       = *p;
   *p        = kk->out;
   kk->out   = tmp;
   /********************************************************/
   /* xor all 32 bits in kk->out to get one output bit     */
   /* the output from ekissbit() performs very well in     */
   /* dieharder -a and TestU01 Crush tests                 */
   /********************************************************/
   bit = 0;          /* initialize the output bit */
   tmp = kk->out;    /* temporary copy of ekiss output */
   i = 32;           /* loop counter for 32 bits */
   while (i--)       /* loop 32 times */
      {
      bit ^= tmp;    /* xor the low order bit to the output bit */
      tmp >>= 1;     /* shift to the next lower order bit */
      } /* for each bit in kk->out */
   /* return output bit */
   return(bit & 1);
   } /* ekissbit subroutine */
