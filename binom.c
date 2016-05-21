/* binom.c - Binomial Distribution Test Version 1.0.0                */
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

/* This program performs a chi square test on a sample      */
/* population in a binomial distribution                    */
/* The program flips a "coin" 16 times and counts the       */
/* number of heads in the tuple                             */
/* The program samples the population 65536 times.          */
/* The expected results are based on Pascal's triangle      */
/* for 2^16                                                 */
/* To determine heads or tails, the ekiss generator is used */
/* by calling ekissbit()                                    */

#include <stdio.h>
#include "ekiss.h"

int main()
   {
   int i;                  /* loop counter for #samples */
   double chisq;           /* chi square total */
   double *p,*q,*r;        /* pointers to actual and expected */
   double actual[32];      /* actual sample totals */
   double expected[32];    /* expected totals */
   kissfmt *kk;            /* ekiss structure */
   /*********************************************************/
   /* initialize total arrays                               */
   /*********************************************************/
   p = (double *) actual;
   q = (double *) actual + 32;
   while (p < q) *p++ = 0.0;
   p = (double *) expected;
   q = (double *) expected + 32;
   while (p < q) *p++ = 0.0;
   /*********************************************************/
   /* set the expected totals based on Pascal's triangle    */
   /*********************************************************/
   p = (double *) expected;
   *p++ = 1.0;
   *p++ = 16.0;
   *p++ = 120.0;
   *p++ = 560.0;
   *p++ = 1820.0;
   *p++ = 4368.0;
   *p++ = 8008.0;
   *p++ = 11440.0;
   *p++ = 12870.0;
   *p++ = 11440.0;
   *p++ = 8008.0;
   *p++ = 4368.0;
   *p++ = 1820.0;
   *p++ = 560.0;
   *p++ = 120.0;
   *p++ = 16.0;
   *p++ = 1.0;
   /*********************************************************/
   /* initialize the ekiss generator                         */
   /*********************************************************/
   kk = (kissfmt *) ekissinit();
   /*********************************************************/
   /* Count actual number of heads                          */
   /*********************************************************/
   i = 65536;          /* loop counter for #samples          */
   while (i--)         /* loop 65536 times                   */
      {
      int j;           /* loop counter for counting heads    */
      int tothd;       /* total number of heads in one tuple */
      tothd = 0;       /* initialize total number of heads   */
      j = 16;          /* set loop counter                   */
      while (j--)
         {
	 if (ekissbit(kk)) tothd++;    /* tally #heads in tuple  */
	 } /* for each flip */
      p = (double *) actual + tothd;   /* point to correct total */
      *p += 1.0;                   /* tally n # of heads */
      } /* for each m-bit sample */
   /*********************************************************/
   /* Calculate chi square                                  */
   /*********************************************************/
   chisq = 0.0;                /* initialize chi square total */
   p = (double *) actual;      /* point to actual array       */
   q = (double *) actual + 17;     /* end of actual array     */
   r = (double *) expected;    /* point to expected array     */
   while (p < q)               /* for each actual total       */
      {
      double diff;             /* difference = actual - expected */
      double diffsq;           /* difference squared             */
      diff = *p - *r;          /* difference = actual - expected */
      diffsq = diff * diff;    /* square the difference          */
      chisq += (diffsq / *r);     /* add to the chi square total */
      p++;                     /* next actual total              */
      r++;                     /* next expected total            */
      } /* for each actual total */
   printf("Binomial Distribution Test\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("Degrees of freedom 16\n");
   printf("95%c of the time, the chi square is "
      "between 6.908 and 28.845\n", '%');
   return(0);
   } /* main */
