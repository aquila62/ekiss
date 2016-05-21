/* poischi.c - Poisson Distribution Chi Square Version 1.0.0         */
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
#include <math.h>
#include <gsl/gsl_cdf.h>
#include "ekiss.h"

typedef struct xxstruct {
   int lambda;
   int sz;
   int tblsz;
   double bigell;
   double enn;
   kissfmt *kk;              /* ekiss structure */
   double *etbl;             /* expected table */
   double *atbl;             /* actual   table */
   } xxfmt;

/* print syntax */
void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s lambda size\n", pgm);
   fprintf(stderr,"Where: lambda is the expected "
      "rate of occurrence\n");
   fprintf(stderr,"Where: size is the number "
      "of uniform fractions generated\n");
   fprintf(stderr,"Example: %s 6 100000\n", pgm);
   exit(1);
   } /* putstx */

int genkay(xxfmt *xx)
   {
   int kay;
   double pee;
   double yuu;
   kay = 0;
   pee = 1.0;
   yuu = 0.0;
   /***********************************/
   /* random arrival                  */
   /***********************************/
   do
      {
      kay++;
      yuu = ekissunif(xx->kk);     /* yuu is uniform # from 0 to 1 */
      pee *= yuu;
      } /* do while... */
   while (pee > xx->bigell);
   return(kay-1);
   } /* genkay */

/* calculate arrival probability */
double pofx(double exx, double lambda)
   {
   double pee;
   double left;
   double rght;
   double diff;
   left = exx * log(lambda);
   rght = lgamma(exx + 1.0);
   diff = left - lambda - rght;
   pee = exp(diff);
   return(pee);
   } /* pofx */

/* build the expected table */
void bldetbl(xxfmt *xx)
   {
   double kay;
   double *p;
   xx->tblsz = 0;
   xx->etbl = (double *) malloc(sizeof(double) * 100);
   if (xx->etbl == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating etbl\n");
      exit(1);
      } /* out of mem */
   kay = 0.0;
   p = (double *) xx->etbl;
   while (1)
      {
      double pee;
      pee = pofx(kay, xx->lambda); 
      if (kay > xx->lambda && pee < 0.0001) break;
      *p++ = pee * xx->enn;
      kay += 1.0;
      xx->tblsz++;
      } /* for each value of x in the distribution */
   } /* bldetbl */

void shwetbl(xxfmt *xx)
   {
   int i;
   double *p,*q;
   i = 0;
   p = (double *) xx->etbl;
   q = (double *) xx->etbl + xx->tblsz;
   while (p < q)
      {
      printf("i %d pofx %.15f\n", i, *p);
      i++;
      p++;
      } /* for each entry in table */
   } /* shwetbl */

/* calculate the chi square */
void chisq(xxfmt *xx)
   {
   double sum;
   double diff;
   double diffsq;
   double minchi;
   double maxchi;
   double df;
   double *p,*q,*r;
   sum = 0.0;
   p = (double *) xx->atbl;
   q = (double *) xx->atbl + xx->tblsz;
   r = (double *) xx->etbl;
   while (p < q)
      {
      diff = *p - *r;
      diffsq = diff * diff;
      sum += (diffsq / *r);
      p++;
      r++;
      } /* for each entry in table */
   df = (double) (xx->tblsz - 1);
   minchi = gsl_cdf_chisq_Pinv(0.025,df);
   maxchi = gsl_cdf_chisq_Pinv(0.975,df);
   printf("Lambda       %d\n", xx->lambda);
   printf("Population   %d\n", xx->sz);
   printf("Chi square   %f\n", sum);
   printf("Deg Freedom  %d\n", xx->tblsz - 1);
   printf("95%c of the time, the chi square ranges "
      "from %f to %f\n", '%', minchi, maxchi);
   } /* chisq */

int main(int argc, char **argv)
   {
   int i;
   double *p,*q;
   xxfmt *xx;
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of mem */
   if (argc != 3) putstx(*argv);
   xx->lambda = atoi(*(argv+1));
   if (xx->lambda < 1)
      {
      fprintf(stderr,"Lambda parameter %s "
         "is too small\n", *(argv+1));
      putstx(*argv);
      exit(1);
      } /* lambda too small */
   if (xx->lambda > 20)
      {
      fprintf(stderr,"Lambda parameter %s "
         "is too large\n", *(argv+1));
      putstx(*argv);
      exit(1);
      } /* lambda too large */
   xx->sz = atoi(*(argv+2));
   if (xx->sz < 36)
      {
      fprintf(stderr,"Size parameter %s "
         "is too small\n", *(argv+2));
      putstx(*argv);
      exit(1);
      } /* sz too small */
   if (xx->sz > 1000000)
      {
      fprintf(stderr,"Size parameter %s "
         "is too large\n", *(argv+2));
      putstx(*argv);
      exit(1);
      } /* sz too large */
   /* initialize the random number generator */
   xx->kk     = (kissfmt *) ekissinit();
   xx->bigell = exp(-xx->lambda);
   xx->enn    = (double) xx->sz;   /* size of the population */
   bldetbl(xx);     /* create the expected table */
   /*****************************/
   /* allocate the actual table */
   /*****************************/
   xx->atbl = (double *) malloc(sizeof(double) * 100);
   if (xx->atbl == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating atbl\n");
      exit(1);
      } /* out of mem */
   /*******************************/
   /* initialize the actual table */
   /*******************************/
   p = (double *) xx->atbl;
   q = (double *) xx->atbl + xx->tblsz;
   while (p < q) *p++ = 0.0;
   /*******************************/
   /* sample the population       */
   /*******************************/
   i = xx->sz;
   while (i--)
      {
      int kay;
      double *p;
      kay = genkay(xx);
      p = (double *) xx->atbl + kay;
      *p += 1.0;
      } /* for each mean generated */
   /*******************************/
   /* compute the chi square      */
   /*******************************/
   chisq(xx);
   /*******************************/
   /* de-allocate memory          */
   /*******************************/
   free(xx->atbl);
   free(xx->etbl);
   free(xx->kk->state);
   free(xx->kk);
   free(xx);
   return(0);              /* normal eoj */
   } /* main */
