/* gendna.c - random DNA generator Version 1.0.0                     */
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
/* This program produces a random sequence of DNA       */
/* base pairs.                                          */
/********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ekiss.h"

#define BUFSZ (1024*1024)

void putblk(char *blk, int len)
   {
   int rslt;
   rslt = write(1,blk,len);
   if (rslt != len)
      {
      fprintf(stderr,"putbyte: write error\n");
      exit(1);
      } /* write error */
   } /* putblk */

int main()
   {
   char *buf;        /* output buffer */
   kissfmt *kk;        /* ekiss structure */
   /*****************************************************/
   /* Alloocate memory for the buffer                   */
   /*****************************************************/
   buf = (char *) malloc(BUFSZ);
   if (buf == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating buf\n");
      exit(1);
      } /* out of memory */
   /*****************************************************/
   kk = (kissfmt *) ekissinit();    /* initialize ekiss RNG */
   /*****************************************************/
   /* write random DNA base pairs to stdout             */
   /*****************************************************/
   /* infinite buffer loop */
   while (1)
      {
      char *p,*q;
      p = (char *) buf;
      q = (char *) buf + BUFSZ;
      while (p < q)
         {
	 if (ekissbit(kk))             /* heads or tails */
	    {
	    if (ekissbit(kk))          /* heads or tails */
	       {
	       *p++ = 'C';
	       *p++ = 'G';
	       } /* generate CG */
	    else
	       {
	       *p++ = 'G';
	       *p++ = 'C';
	       } /* generate GC */
	    } /* generate CG or GC */
	 else
	    {
	    if (ekissbit(kk))          /* heads or tails */
	       {
	       *p++ = 'A';
	       *p++ = 'T';
	       } /* generate AT */
	    else
	       {
	       *p++ = 'T';
	       *p++ = 'A';
	       } /* generate TA */
	    } /* generate AT or TA */
	 } /* for each DNA base pair in the buffer */
      putblk(buf,BUFSZ);
      } /* for each block of random DNA base pairs created */
   free(buf);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
