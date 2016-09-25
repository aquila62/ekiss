/************************************************************************/
/* pair.c  Version 1.0.0                                                */
/* Split DNA base pairs into parallel strands                           */
/* Copyright (C) 2016  aquila62 at github.com                           */

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

#define BUFSZ (1024*1024)

/* read one block from stdin */
int getblk(unsigned char *blk)
   {
   int len;
   len = read(0,blk,BUFSZ);
   if (!len) return(EOF);
   if (len < 0)
      {
      fprintf(stderr,"getbyte: read error\n");
      exit(1);
      } /* read error */
   return(len);
   } /* getblk */

int main()
   {
   unsigned char *buf;       /* input buffer */
   /*****************************************************/
   /* Alloocate memory for the input buffer             */
   /*****************************************************/
   buf = (unsigned char *) malloc(BUFSZ);
   if (buf == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating buf\n");
      exit(1);
      } /* out of memory */
   /*****************************************************/
   /* split the DNA base pairs into parallel strands    */
   /* loop terminated by end of input stream            */
   /*****************************************************/
   while (1)
      {
      int blklen;             /* actual block length */
      char topstr[128];       /* left strand */
      char botstr[128];       /* right strand */
      char *topp;             /* pointer to left strand */
      char *topq;             /* pointer to left strand */
      char *botp;             /* pointer to right strand */
      char *p,*q;             /* buffer pointers */
      blklen = getblk(buf);   /* read a block from stdin */
      if (blklen == EOF) break;   /* break if no more input */
      p = (char *) buf;           /* point to start of buffer */
      q = (char *) buf + blklen;    /* point to end of buffer */
      while (p < q)
         {
	 topp = (char *) topstr;    /* point to start of left strand */
	 topq = (char *) topstr + 72;  /* point to end of left strand */
	 botp = (char *) botstr;    /* point to start of right strand */
	 /* split DNA base pairs into parallel strands */
	 /* print only 72 pairs per line */
	 while (topp < topq)
	    {
	    *topp++ = *p++;
	    *botp++ = *p++;
	    } /* for each character in line */
	 *topp = '\0';
	 *botp = '\0';
	 printf("%s\n%s\n--------\n",
	    topstr, botstr);
	 } /* for each DNA base pair in the buffer */
      } /* for each block of input */
   free(buf);
   return(0);
   } /* main */
