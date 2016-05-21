/* ekissinit.c - Extended kiss initialization Version 1.0.0          */
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

/* This is the initialization routine for extended kiss    */
/* This initialization routine is based on date/time/ticks */
/* The caller is welcome to override the following:        */
/* the state array                                         */
/* the two previous outputs                                */
/* the current output                                      */
/* the three states for the kiss algorithm                 */
/*                                                         */
/* The period length of ekiss is estimated to be           */
/* 5.41e+2639.                                             */
/* The period length of GSL kiss2 is 3.09e+26.             */
/* The speed of ekiss is 1/6 faster than the GSL version   */
/* of kiss2.                                               */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include "ekiss.h"

/* size of the state array is 2^10 */
#define STATES 1024

kissfmt *ekissinit(void)
   {
   int i;                      /* loop counter */
   unsigned int dttk;          /* combined date, time, and #ticks */
   unsigned int *stp,*stq;     /* pointer into state array */
   time_t now;                 /* current date and time */
   clock_t clk;                /* current number of ticks */
   struct tms t;               /* structure used by times() */
   kissfmt *kk;                /* ekiss structure */

   /***************************************************/
   /* allocate memory for ekiss structure */
   /***************************************************/
   kk = (kissfmt *) malloc(sizeof(kissfmt));
   if (kk == NULL)
      {
      fprintf(stderr,"ekissinit: out of memory "
      "allocating ekiss structure kk\n");
      exit(1);
      } /* out of memory */
   kk->states = STATES; /* save the number of ekiss states */

   /***************************************************/
   /* allocate memory for ekiss state array           */
   /***************************************************/
   kk->state = (unsigned int *)
      malloc(sizeof(unsigned int) * STATES);
   if (kk->state == NULL)
      {
      fprintf(stderr,"ekissinit: out of memory "
      "allocating kk->state state array\n");
      exit(1);
      } /* out of memory */

   /***************************************************/
   /* Randomize the seeds and states                  */
   /***************************************************/
   /* get clock ticks since boot                           */
   clk = times(&t);
   /* get date & time                                      */
   time(&now);
   /* combine date, time, and ticks into a single UINT     */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the three kiss states to date,time,ticks  */
   kk->z     = now;      /* initialize z  to date, time        */
   kk->w     = clk;      /* initialize w  to ticks             */
   kk->jsr   = dttk;     /* initialize jsr to combined dttk    */
   /* initialize jcong to combined dttk permutation            */
   kk->jcong = ((dttk>>16) | (dttk<<16));

   /***************************************************/
   /* Warm up the kiss states.                        */
   /***************************************************/
   i = 128;
   while (i--) kk->out = KISS;

   /***************************************************/
   /* initialize the state array to random values     */
   /***************************************************/
   stp = (unsigned int *) kk->state;
   stq = (unsigned int *) kk->state + STATES;
   while (stp < stq)
      {
      *stp++ = KISS;            /* set to random UINT */
      } /* for each member in kk->state array */

   /***************************************************/
   /* initialize out, prev, and prev prev             */
   /* to random values                                */
   /***************************************************/
   kk->pprev = KISS;            /* set to random UINT */
   kk->prev  = KISS;            /* set to random UINT */
   kk->out   = KISS;            /* set to random UINT */

   /***************************************************/
   /* after this subroutine you may initialize the    */
   /* state array to your own values, if you wish     */
   /* to do regression testing.                       */
   /* Use the above 12 instructions as an example of  */
   /* how to initialize the generator                 */
   /***************************************************/
   /* return the ekiss structure                      */
   /***************************************************/
   return(kk);
   } /* ekissinit subroutine */
