/* ekisschi.c - ekiss chi square test Version 1.0.0                    */
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ekiss.h"

int main()
   {
   int i;                         /* loop counter */
   double *p,*q;                  /* pointers to tally array */
   double tally[32];              /* chi square counts */
   double expected = 4096.0;      /* expected chi square count */
   double diff;                   /* expected - actual count */
   double diffsq;                 /* difference squared */
   double chisq = 0.0;            /* chi square total */
   kissfmt *kk;                   /* ekiss structure */
   kk = (kissfmt *) ekissinit();
   /**********************************/
   /* initialize tally array to zero */
   /**********************************/
   p = (double *) tally;
   q = (double *) tally + 32;
   while (p < q) *p++ = 0.0;
   /********************************************/
   /* tally 65536 4-bit samples in 16 counters */
   /********************************************/
   i = 65536;
   while (i--)
      {
      int indx;
      indx = ekisspwr(kk,4);      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p++;           /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("Uniform Distribution Chi Square Test\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   free(kk->state);
   free(kk);
   return(0);            /* normal eoj */
   } /* main */
