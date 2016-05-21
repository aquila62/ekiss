/* ekiss.c - random number generator Version 1.0.0                    */
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
/* ekiss: "extended kiss"                               */
/* This subroutine produces a random unsigned           */
/* 32-bit integer using the kiss algorithm plus a       */
/* Bays-Durham shuffle.  The previous two cycles are    */
/* xor'd together with the current cycle.               */
/* The speed of ekiss is equivalent to etaus.           */
/********************************************************/

#include "ekiss.h"

unsigned int ekiss(kissfmt *kk)
   {
   unsigned int *p;           /* state array pointer */
   unsigned int tmp;          /* used for Bays-Durham shuffle */
   /**********************************************************/
   /* The offset into the state array is not related         */
   /* to the current state.                                  */
   /**********************************************************/
   /* calculate a 10-bit offset into the state array         */
   /* See the Bays-Durham shuffle below.                     */
   kk->ofst  = kk->pprev >> 22;   /* offset into state array */
   kk->pprev = kk->prev;   /* prev prev <== prev             */
   kk->prev  = kk->out;    /* prev <== current               */
   /* calculate new current state                            */
   /* using the kiss algorithm                               */
   /* The KISS macro is in ekiss.h                           */
   /* XOR the two previous outputs with the current output   */
   kk->out = (KISS ^ kk->prev ^ kk->pprev);

   /********************************************************/
   /* Bays-Durham shuffle of state array                   */
   /* 1024! = 5.41e+2639 base 10 rounded down              */
   /* The period length of the state array is theoretical  */
   /* and cannot be proven with empirical testing.         */
   /********************************************************/
   /* point to a state array element                       */
   p       = (unsigned int *) kk->state + kk->ofst;
   /* swap the current output with the member of the state array */
   tmp     = *p;
   *p      = kk->out;
   kk->out = tmp;
   /********************************************************/
   /* return the output of the ekiss cycle after the swap  */
   return(kk->out);
   } /* ekiss subroutine */
