/* uintchi.c - UINT chi square test Version 1.0.0                    */
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

int main(void)
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
      indx = (ekiss(kk) >> 28) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("Uniform Distribution Chi Square Test Upper 4 bits\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 25-28                       */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 24) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 25-28\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 21-24                       */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 20) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 21-24\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 17-20                       */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 16) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 17-20\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 13-16                       */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 12) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 13-16\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 9-12                        */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 8) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 9-12\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 5-8                         */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = (ekiss(kk) >> 4) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 5-8\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   /*******************************************************/
   /* chi square test on bits 1-4                         */
   /*******************************************************/
   /* initialize tally array to zero                      */
   /*******************************************************/
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
      indx = ekiss(kk) & 15;      /* 4 bit sample 0-15 */
      tally[indx] += 1.0;        /* tally the sample */
      } /* for each sample */
   /******************************************/
   /* add up the chi square from 16 counters */
   /******************************************/
   chisq = 0.0;
   p = (double *) tally;
   q = (double *) tally + 16;
   i = 16;
   while (i--)
      {
      diff = expected - *p;             /* expected - actual */
      diffsq = diff * diff;             /* square the difference */
      chisq += (diffsq / expected);     /* add to chi square total */
      p++;
      } /* for each tally */
   /****************/
   /* print report */
   /****************/
   printf("-----------------------------------------------\n");
   printf("Uniform Distribution Chi Square Test bits 1-4\n");
   printf("Total samples 65536\n");
   printf("Chi square %f\n", chisq);
   printf("15 degrees of freedom\n");
   printf("95%c of the time, the chi square is "
      "between 6.262 and 27.488\n", '%');
   free(kk->state);
   free(kk);
   return(0);            /* normal eoj */
   } /* main */
