/************************************************************************/
/* genalfa.c  Version 1.0.0                                             */
/* Generate an alpha text stream                                        */
/* Copyright (C) 2016  Russell Stevens                                  */

/* This program is free software; you can redistribute it and/or        */
/* modify it under the terms of the GNU General Public License as       */
/* published by the Free Software Foundation; either version 2 of       */
/* the License, or (at your option) any later version.                  */

/* This program is distributed in the hope that it will be useful,      */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */

/* You should have received a copy of the GNU General Public License    */
/* along with this program; if not, write to:                           */

	/* Free Software Foundation, Inc.         */
	/* 59 Temple Place - Suite 330            */
	/* Boston, MA  02111-1307, USA.           */
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ekiss.h"

#define BUFSZ (1024*1024)

/* print the command syntax */
void putstx(char *pgm)
   {
   fprintf(stderr,"Usage: %s alphabet\n", pgm);
   fprintf(stderr,"Where alphabet is an ASCII string\n");
   fprintf(stderr,"Example: %s abc\n", pgm);
   fprintf(stderr,"Example: %s \"abcdefghijklmnopqrstuvwxyz\"\n", pgm);
   fprintf(stderr,"Example: %s '0123456789'\n", pgm);
   exit(1);
   } /* putstx */

/* write one block of output to stdout */
/* the output block is unsigned because */
/* it is possible that the alphabet contains */
/* accented characters 0xa0 to 0xfe */
void putblk(unsigned char *blk, int len)
   {
   int rslt;
   rslt = write(1,blk,len);
   if (rslt != len)
      {
      fprintf(stderr,"putblk: write error\n");
      exit(1);
      } /* write error */
   } /* putblk */

int main(int argc, char **argv)
   {
   int alflen;          /* length of alphabet parameter */
   unsigned char alfabet[256];    /* alphabet to generate text */
   unsigned char *buf;        /* output buffer */
   kissfmt *kk;               /* ekiss RNG structure */
   if (argc != 2) putstx(*argv);   /* must have alphabet parameter */
   /****************************/
   /* validate alphabet length */
   /****************************/
   alflen = strlen(*(argv+1));
   if (alflen > 255)
      {
      fprintf(stderr,"main: alphabet overflow\n");
      fprintf(stderr,"password is 1-255 bytes long\n");
      putstx(*argv);
      } /* password overflow */
   strcpy((char *) alfabet,*(argv+1));    /* save alphabet */
   /*****************************************************/
   /* Alloocate memory for the buffer                   */
   /*****************************************************/
   buf = (unsigned char *) malloc(BUFSZ);
   if (buf == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating buf\n");
      exit(1);
      } /* out of memory */
   /*****************************************************/
   kk = (kissfmt *) ekissinit();    /* initialize ekiss RNG */
   /*****************************************************/
   /* write random text to stdout                       */
   /*****************************************************/
   /* infinite block loop */
   while (1)
      {
      int ofst;
      unsigned char *p,*q;
      p = (unsigned char *) buf;
      q = (unsigned char *) buf + BUFSZ;
      while (p < q)
         {
	 unsigned char *alfap;   /* pointer into alphabet */
         ofst = ekissint(kk,alflen);   /* alphabet offset */
	 /* pick a random character from the alphabet */
         alfap = (unsigned char *) alfabet + ofst;
	 /* append the random character to the output buffer */
         *p++ = *alfap;
	 } /* for each byte in the buffer */
      putblk(buf,BUFSZ);    /* write the buffer to stdout */
      } /* for each block of random characters sent */
   free(buf);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
