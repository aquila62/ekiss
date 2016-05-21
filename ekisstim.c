/* ekisstim.c - time the ekiss RNG  Version 1.0.0                      */
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
#include <time.h>
#include <sys/times.h>
#include <gsl/gsl_rng.h>
#include "ekiss.h"

int main(void)
   {
   int elap;
   unsigned int dttk;
   double count;
   time_t now;
   clock_t clk;
   clock_t start;
   clock_t fin;
   struct tms t;
   kissfmt *kk;
   gsl_rng *r;
   kk = (kissfmt *) ekissinit();
   /**************************************************/
   count = 0.0;
   start = times(&t);       /* get ticks */
   while (1)
      {
      ekiss(kk);
      count += 1.0;
      if (count >= 100000000.0)
         {
         fin = times(&t);
	 elap = (fin - start); 
	 break;
	 } /* if 100 million cycles */
      } /* for each cycle */
   printf("ekiss                count %.0f ticks %d\n",
      count, elap);
   /**************************************************/
   /* time the GSL taus2 RNG                         */
   /**************************************************/
   /* initialize the GSL taus2 RNG                   */
   /* to date, time, #ticks                          */
   /**************************************************/
   r = (gsl_rng *) gsl_rng_alloc(gsl_rng_taus2);
   /* get clock ticks since boot                       */
   clk = times(&t);
   /* get date & time                                  */
   time(&now);
   /* combine date, time, and ticks into a single UINT */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the GSL taus2 random number generator */
   /* to date,time,#ticks                              */
   gsl_rng_set(r, dttk);
   /**************************************************/
   count = 0.0;
   /* get ticks */
   start = times(&t);
   while (1)
      {
      gsl_rng_get(r);
      count += 1.0;
      if (count >= 100000000.0)
         {
         fin = times(&t);
	 elap = (fin - start); 
	 break;
	 } /* if 100 million cycles */
      } /* for each cycle */
   printf("GSL taus2            count %.0f ticks %d\n",
      count, elap);
   gsl_rng_free(r);
   /**************************************************/
   /* time the GSL Mersenne Twister RNG              */
   /**************************************************/
   /* initialize the GSL Mersenne Twister RNG        */
   /* to date, time, #ticks                          */
   /**************************************************/
   r = (gsl_rng *) gsl_rng_alloc(gsl_rng_mt19937);
   /* get clock ticks since boot                       */
   clk = times(&t);
   /* get date & time                                  */
   time(&now);
   /* combine date, time, and ticks into a single UINT */
   dttk = (unsigned int) (now ^ clk);
   /* initialize the GSL mt19937 random number generator */
   /* to date,time,#ticks                              */
   gsl_rng_set(r, dttk);
   /**************************************************/
   count = 0.0;
   /* get ticks */
   start = times(&t);
   while (1)
      {
      gsl_rng_get(r);
      count += 1.0;
      if (count >= 100000000.0)
         {
         fin = times(&t);
	 elap = (fin - start); 
	 break;
	 } /* if 100 million cycles */
      } /* for each cycle */
   printf("GSL Mersenne Twister count %.0f ticks %d\n",
      count, elap);
   gsl_rng_free(r);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
