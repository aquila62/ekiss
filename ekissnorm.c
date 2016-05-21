/* ekissnorm.c - ekiss Gaussian distribution Version 1.0.0           */
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
#include "ekiss.h"

/* this tree structure is used to sort the */
/* output of the Box-Muller transform      */
typedef struct treestruct {
   struct treestruct *left;
   struct treestruct *rght;
   double key;
   } treefmt;

/* actual normal distribution sample structure */
typedef struct actstr {
   double n1;
   double n2;
   } actfmt;

/* print syntax */
void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s mean stdev size\n", pgm);
   fprintf(stderr,"Where: mean is the expected "
      "mean of the random variable\n");
   fprintf(stderr,"Where: stdev is the expected "
      "standard deviation of the random variable\n");
   fprintf(stderr,"Where: size is the number "
      "of normal numbers generated\n");
   fprintf(stderr,"Example: %s 15.0 2.0 10000\n", pgm);
   exit(1);
   } /* putstx */

/* See Box-Muller Transform in Wikipedia */
void boxmul(kissfmt *kk, double mean,
   double stdev, actfmt *aa)
   {
   double u1;
   double u2;
   double x1;
   double x2;
   double twopi;
   double rootu1;
   u1 = ekissunif(kk);
   u2 = ekissunif(kk);
   twopi = 2.0 * M_PI;
   rootu1 = sqrt(-2.0 * log(u1));
   x1 = rootu1 * cos(twopi * u2);
   x2 = rootu1 * sin(twopi * u2);
   aa->n1 = mean + (stdev * x1);
   aa->n2 = mean + (stdev * x2);
   } /* boxmul */

/* initialize a tree node */
treefmt *inittree(void)
   {
   treefmt *newnode;
   newnode = (treefmt *) malloc(sizeof(treefmt));
   if (newnode == NULL)
      {
      fprintf(stderr,"inittree: out of memory\n");
      exit(1);
      } /* out of mem */
   newnode->left = NULL;
   newnode->rght = NULL;
   newnode->key  = 0.0;
   return(newnode);
   } /* inittree */

/* insert a normal number as a key into the tree */
/* duplicate key is an error                     */
/* this is a recursive routine.                  */
int isrt(treefmt *node, double key)
   {
   int rslt;
   treefmt *newnode;
   if (key < node->key)
      {
      if (node->left != NULL)
	 {
         rslt = isrt(node->left, key);
	 return(rslt);
	 } /* not leaf node left */
      else
         {
	 newnode = (treefmt *) inittree();
	 newnode->key = key;
	 node->left = (treefmt *) newnode;
	 return(0);
	 } /* else leaf node less than */
      } /* key less than */
   else if (key > node->key)
      {
      if (node->rght != NULL)
	 {
         rslt = isrt(node->rght, key);
	 return(rslt);
	 } /* not leaf node rght */
      else
         {
	 newnode = (treefmt *) inittree();
	 newnode->key = key;
	 node->rght = (treefmt *) newnode;
	 return(0);
	 } /* else leaf node greater than */
      } /* key greater than */
   return(-1);   /* duplicate key */
   } /* isrt */

/* read the tree in in-order sequence */
/* this is a recursive routine.       */
void traverse(treefmt *node)
   {
   if (node->left != NULL) traverse(node->left);
   printf("%.15f\n", node->key);
   if (node->rght != NULL) traverse(node->rght);
   } /* traverse */

/* delete a tree node recursively */
void rmtree(treefmt *node)
   {
   if (node->left != NULL) rmtree(node->left);
   if (node->rght != NULL) rmtree(node->rght);
   free(node);
   } /* rmtree */

int main(int argc, char **argv)
   {
   int sz;                  /* number of normal numbers to produce */
   unsigned int i;          /* loop countedr                   */
   double mean;             /* mean of the normal distribution */
   double stdev;            /* standard deviation              */
   kissfmt *kk;             /* ekiss structure                 */
   actfmt *aa;              /* Box-Muller transform structure  */
   treefmt *root;           /* root node of the tree           */
   if (argc != 4) putstx(*argv);       /* must have 3 arguments */
   mean = atof(*(argv+1));             /* sample mean */
   if (mean < -1000.0)
      {
      fprintf(stderr,"Mean parameter %s "
         "is too small\n", *(argv+1));
      putstx(*argv);
      exit(1);
      } /* mean too small */
   if (mean > 1000.0)
      {
      fprintf(stderr,"Mean parameter %s "
         "is too large\n", *(argv+1));
      putstx(*argv);
      exit(1);
      } /* mean too large */
   stdev = atof(*(argv+2));            /* standard deviation */
   if (stdev < 1.0)
      {
      fprintf(stderr,"Stdev parameter %s "
         "is too small\n", *(argv+2));
      putstx(*argv);
      exit(1);
      } /* stdev too small */
   if (stdev > 1000.0)
      {
      fprintf(stderr,"Stdev parameter %s "
         "is too large\n", *(argv+2));
      putstx(*argv);
      exit(1);
      } /* stdev too large */
   sz = atoi(*(argv+3));                 /* population size */
   if (sz < 36)
      {
      fprintf(stderr,"Size parameter %s "
         "is too small\n", *(argv+3));
      putstx(*argv);
      exit(1);
      } /* sz too small */
   if (sz > 100000)
      {
      fprintf(stderr,"Size parameter %s "
         "is too large\n", *(argv+3));
      putstx(*argv);
      exit(1);
      } /* sz too large */
   kk = (kissfmt *) ekissinit();   /* initialize ekiss to date/time */
   /* allocate memory for the actual sample structure */
   aa = (actfmt *) malloc(sizeof(actfmt));
   if (aa == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating the aa structure\n");
      exit(1);
      } /* out of memory */
   root    = (treefmt *) inittree();
   /**********************************/
   /* create n samples               */
   /**********************************/
   i = sz >> 1;
   while (i--)
      {
      boxmul(kk,mean,stdev,aa);        /* Box-Muller Transform */
      /* the Box-Muller Transform gives two numbers n1 and n2 */
      /* tally by number to create histogram */
      isrt(root,aa->n1);      /* insert n1 into the tree */
      isrt(root,aa->n2);      /* insert n2 into the tree */
      } /* for each sample */
   /**********************************/
   /* traverse tree                  */
   /**********************************/
   if (root->rght != NULL) traverse(root->rght);
   /************************************************************/
   /* free memory                                              */
   /************************************************************/
   if (root->rght != NULL) rmtree(root->rght);
   free(root);
   free(aa);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
