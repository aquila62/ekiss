/* mcsin.c - Monte Carlo Integral of sin(x) Version 1.0.0            */
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

/***********************************************************/
/* mcsin estimates the integral of sin(x) through a Monte  */
/* Carlo simulation.                                       */
/* x = uniform random number 0-pi                          */
/* y = uniform random number 0-1                           */
/* n = 1 million                                           */
/* If y < sin(x) then tally a counter                      */
/* Pi * the ratio of the (counter / 1 million)             */
/* is approximately 2.0 to 3 decimal places                */
/* or approximately 2.000...                               */
/***********************************************************/

#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include "ekiss.h"

int main(void)
   {
   int i;                     /* loop counter                 */
   int ekisselap;             /* elapsed time for ekiss       */
   int mtelap;                /* elapsed time for mt19937     */
   int ranlxelap;             /* elapsed time for ranlxd2     */
   unsigned int dttk;         /* combined date, time #ticks   */
   double tot;                /* total points within a circle */
   double bot;                /* 1 million                    */
   double ratio;              /* estimated 2 / pi             */
   time_t now;                /* current date and time        */
   clock_t clk;               /* current number of ticks      */
   clock_t ekissstart;        /* start time for ekiss         */
   clock_t ekissfin;          /* end   time for ekiss         */
   clock_t mtstart;           /* start time for mt19937       */
   clock_t mtfin;             /* end   time for mt19937       */
   clock_t ranlxstart;        /* start time for ranlxd2       */
   clock_t ranlxfin;          /* end   time for ranlxd2       */
   struct tms t;              /* structure used by times()    */
   gsl_rng *r;                /* GSL RNG structure            */
   kissfmt *kk;               /* ekiss structure              */
   kk = (kissfmt *) ekissinit();     /* initialize the ekiss structure */
   bot = 1000000.0;           /* set to 1 million             */
   /************************************************************/
   tot = 0.0;                 /* initialize total */
   i = (int) bot;             /* set loop counter */
   /* get clock ticks since boot                           */
   ekissstart = times(&t);    /* start time for ekiss      */
   while (i--)                /* loop 1 million times      */
      {
      double x;               /* horizontal coordinate        */
      double y;               /* vertical   coordinate        */
      double wyy;             /* y = sin(x)                   */
      x = ekissunif(kk) * M_PI;   /* uniform number 0-pi      */
      y = ekissunif(kk);      /* uniform number 0-1           */
      wyy = sin(x);           /* the sine curve */
      if (y < wyy) tot += 1.0;   /* if y is under the curve, tally */
      } /* for each point above or below a sine curve */
   ratio = M_PI * (tot / bot);       /* calculate est. 2.0    */
   ekissfin = times(&t);      /* finish time for ekiss */
   printf("Monte Carlo Definite Integral of sin(x)\n");
   printf("      From zero to Pi\n");
   printf("      Expected error is 1/1000\n");
   printf("      n = 1 million\n");
   printf("      ekiss %18.15f\n", ratio);
   /************************************************************/
   /* allocate the mt19937 random number generator */
   r = (gsl_rng *) gsl_rng_alloc(gsl_rng_mt19937);
   /* get clock ticks since boot                           */
   clk = times(&t);
   /* get date & time                                      */
   time(&now);
   /* combine date, time, and ticks into a single UINT     */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the GSL Mersenne Twister                     */
   /* random number generator to date,time,#ticks             */
   gsl_rng_set(r, dttk);      /* initialize mt19937 seed      */
   tot = 0.0;                 /* initialize total */
   i = (int) bot;             /* set loop counter */
   /* get clock ticks since boot                              */
   mtstart = times(&t);       /* start time for GSL mt19937   */
   while (i--)                /* loop 1 million times         */
      {
      double x;               /* horizontal coordinate        */
      double y;               /* vertical   coordinate        */
      double wyy;             /* y = sin(x)                   */
      /* use the mt19937 random number generator this time    */
      x = gsl_rng_uniform(r) * M_PI;  /* uniform number 0-pi  */
      y = gsl_rng_uniform(r);      /* uniform number 0-1      */
      wyy = sin(x);           /* the sine curve */
      if (y < wyy) tot += 1.0;   /* if under the curve, tally */
      } /* for each point above or below a sine curve */
   ratio = M_PI * tot / bot;         /* calculate est. 2.0     */
   mtfin = times(&t);         /* finish time for GSL mt19937   */
   printf("GSL mt19937 %18.15f\n", ratio);
   /************************************************************/
   /* allocate the ranlxd2 random number generator */
   r = (gsl_rng *) gsl_rng_alloc(gsl_rng_ranlxd2);
   /* get clock ticks since boot                           */
   clk = times(&t);
   /* get date & time                                      */
   time(&now);
   /* combine date, time, and ticks into a single UINT     */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the GSL ranlxd2 random number generator      */
   /* to date,time,#ticks                                     */
   gsl_rng_set(r, dttk);      /* initialize ranlxd2 seed      */
   tot = 0.0;                 /* initialize total */
   i = (int) bot;             /* set loop counter */
   /* get clock ticks since boot                              */
   ranlxstart = times(&t);    /* start time for GSL ranlxd2   */
   while (i--)                /* loop 1 million times         */
      {
      double x;               /* horizontal coordinate        */
      double y;               /* vertical   coordinate        */
      double wyy;             /* y = sin(x)                   */
      /* use the ranlxd2 random number generator this time    */
      x = gsl_rng_uniform(r) * M_PI;   /* uniform number 0-pi */
      y = gsl_rng_uniform(r);      /* uniform number 0-1      */
      wyy = sin(x);           /* the sine curve */
      if (y < wyy) tot += 1.0;   /* if under the curve, tally */
      } /* for each point above or below a sine curve */
   ratio = M_PI * (tot / bot);       /* calculate est. 2.0     */
   ranlxfin = times(&t);      /* finish time for GSL ranlxd2   */
   printf("GSL ranlxd2 %18.15f\n", ratio);
   printf("     Actual %18.15f\n", 2.0);
   ekisselap = ekissfin - ekissstart;
   mtelap    = mtfin    - mtstart;
   ranlxelap = ranlxfin - ranlxstart;
   printf("      ekiss ticks %6d\n", ekisselap);
   printf("GSL mt19937 ticks %6d\n", mtelap);
   printf("GSL ranlxd2 ticks %6d\n", ranlxelap);
   return(0);
   } /* main */
