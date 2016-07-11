/* captcha.c - captcha generator Version 1.0.0                       */
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

/* create a captcha picture with a random 16 million color */
/* pixel background */
/* the background in the captcha contains pixels */
/* 0-255,0-255,0-255 */
/* the text in the captcha contains pixels */
/* 0-63,0-63,0-63 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ekiss.h"

#define MAXLEN (512*512)
#define TXTLEN (256)

/* read one byte of text from stdin */
int getbyte()
   {
   int len;
   char buf[8];
   len = read(0,buf,1);
   if (!len) return(EOF);
   if (len != 1)
      {
      fprintf(stderr,"getbyte: read error\n");
      exit(1);
      } /* read error */
   return(buf[0]);
   } /* getbyte */

/* read one byte from the font file */
int gethndl(int hndl)
   {
   int len;
   char buf[8];
   len = read(hndl,buf,1);
   if (!len) return(EOF);
   if (len != 1)
      {
      fprintf(stderr,"gethndl: read error "
         "handle=%d\n", hndl);
      exit(1);
      } /* read error */
   return(buf[0]);
   } /* gethndl */

/* open the font file */
int opn(char *fname)
   {
   int hndl;
   hndl = open(fname,O_RDONLY);
   if (hndl < 0)
      {
      fprintf(stderr,"opn: open error %s\n", fname);
      exit(1);
      } /* open error */
   return(hndl);
   } /* opn */

/* close the font file */
void cls(int hndl)
   {
   int rslt;
   rslt = close(hndl);
   if (rslt < 0)
      {
      fprintf(stderr,"cls: close error %d\n", hndl);
      exit(1);
      } /* close error */
   } /* cls */

/* write the PPM prefix and the PPM chunk in separate calls */
/* the output is to stdout */
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

/* debugging routine to display the font in memory */
void shwfnt(char *fnt, int fntlen)
   {
   int rowsz;
   char *p,*q;
   rowsz = 0;
   p = (char *) fnt;
   q = (char *) fnt + fntlen;
   while (p < q)
      {
      fprintf(stderr,"%c", *p++);
      rowsz++;
      if (rowsz >= 12)
         {
	 fprintf(stderr,"\n");
	 rowsz = 0;
	 } /* if end of row */
      } /* for each letter in font */
   } /* shwfnt */

int main()
   {
   int row;         /* row# in the picture */
   int col;         /* column# in the picture */
   int txtindx;     /* index into the text */
   int fntofst;     /* offset into the 12x24 font */
   int picofst;     /* offset into the picture */
   int fntlen;      /* the total length of the font (12*24*94) */
   int txtlen;      /* the total length of the text */
   int pfxlen;      /* the length of the PPM prefix */
   int picrowsz;    /* the number of pixels*3 in a PPM row */
   int piccolsz;    /* the number of rows in the PPM */
   int picsz;       /* the number of pixels*3 in the PPM */
   int hndl;        /* the font file handle */
   char *fnt;       /* pointer for the font */
   char *fntp,*fntq;       /* relative pointers for the font */
   char *p,*q;       /* relative pointers for the text */
   char *txt;        /* pointer for the font */
   char *txtp;       /* relative pointer for the text */
   /* format for the PPM prefix: */
   /* P6 */
   /* white space */
   /* row size in pixels */
   /* white space */
   /* column size in pixels */
   /* white space */
   /* colors depth = 255 */
   /* white space or new line */
   char pfx[64];     /* PPM prefix */
   unsigned char *pic;      /* pointer to the PPM */
   unsigned char *picp;     /* relative pointer for the PPM */
   unsigned char *picq;     /* relative pointer for the PPM */
   /* the ekiss random number generator uses */
   /* the KISS algorithm created by Prof. George Marsaglia */
   /* plus the Bays-Durham shuffle */
   kissfmt *kk;               /* ekiss structure */
   kk = (kissfmt *) ekissinit();    /* initialize the ekiss generator */
   /*******************************************/
   /* allocate the font space                 */
   /*******************************************/
   fnt = (char *) malloc(MAXLEN);
   if (fnt == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating fnt\n");
      exit(1);
      } /* out of memory */
   /*******************************************/
   /* allocate the text space                 */
   /*******************************************/
   txt = (char *) malloc(TXTLEN);
   if (txt == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating txt\n");
      exit(1);
      } /* out of memory */
   /*********************************************/
   /* open the font file                        */
   /* read the font into memory                 */
   /* the font is an ASCII font of asterisks    */
   /* the asterisks are translated into pixels  */
   /*********************************************/
   hndl = opn("font.txt");
   fntlen = 0;
   fntp = (char *) fnt;
   fntq = (char *) fnt + MAXLEN;
   while (fntp < fntq)
      {
      int ch;
      ch = gethndl(hndl);
      if (ch == EOF) break;       /* end of file */
      if (ch == '\n') continue;   /* bypass end of line */
      *fntp++ = (char) ch;
      fntlen++;
      } /* for each pixel in font */
   cls(hndl);         /* close the font file */
   /* re-allocate the font space */
   fnt = (char *) realloc(fnt,fntlen);
   /* display the font, for debugging */
   /************ shwfnt(fnt,fntlen); *******************/
   /*********************************************/
   /* read the captcha text from stdin          */
   /* bypass end of line                        */
   /* allow only printable characters           */
   /* ASCII space to '~'                        */
   /*********************************************/
   txtlen = 0;
   p = (char *) txt;
   q = (char *) txt + TXTLEN - 1;
   while (p < q)
      {
      int ch;
      ch = getbyte();
      if (ch == EOF) break;
      if (ch == '\n') break;
      if (ch < ' ' || ch > '~')
         {
	 fprintf(stderr,"main: invalid text\n");
	 fprintf(stderr,"char %02x   char# %d\n",
	    ch, txtlen+1);
	 exit(1);
	 } /* invalid txt char */
      *p++ = (char) ch;
      txtlen++;
      } /* for each char in text */
   *p = '\0';        /* terminate the text string */
   /* allow three pixels between each character */
   /* the font is 12x24 */
   /* txtlen * 15 because of 3 dots in between */
   /* allow variable margin space for rows and columns */
   /* the PPM has a partially random size */
   /* each pixel in the PPM contains three RGB bytes */
   picrowsz = 3 * ((txtlen*15) + 80 + (ekissint(kk,40)));
   piccolsz = 24 + 40 + (ekissint(kk,40));
   picsz = picrowsz * piccolsz;    /* total PPM size */
   /* write the dimensions of the PPM to stderr */
   fprintf(stderr,"txtlen %d picrowsz %d piccolsz %d\n",
      txtlen, picrowsz, piccolsz);
   /*******************************************/
   /* allocate the PPM space                  */
   /*******************************************/
   pic = (unsigned char *) malloc(picsz + 1024);
   if (pic == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating pic\n");
      exit(1);
      } /* out of memory */
   /*******************************************/
   /* write the PPM prefix to stdout          */
   /*******************************************/
   sprintf(pfx,"P6 %d %d 255\n",
      picrowsz/3, piccolsz);
   pfxlen = strlen(pfx);
   putblk((unsigned char *) pfx,pfxlen);
   /*******************************************/
   /* generate the random PPM background      */
   /* in 16 million color RGB pixels          */
   /*******************************************/
   picp = (unsigned char *) pic;
   picq = (unsigned char *) pic + picsz;
   while (picp < picq)
      {
      int i;
      i = ekisspwr(kk,8);
      *picp++ = (unsigned char) i;
      } /* for each rgb in pixel */
   /*******************************************/
   /* overlay the random background with      */
   /* partially random colored text           */
   /* the text is darker than the background  */
   /*******************************************/
   txtindx = 0;
   /* loop for each character in text */
   while (txtindx < txtlen)
      {
      int ch;
      txtp = (char *) txt + txtindx;
      ch = *txtp;
      /* space is random background */
      if (ch == ' ')
         {
	 txtindx++;
	 continue;
	 } /* bypass space */
      /* point to the font for the character ch */
      fntofst = (ch - '!') * (12 * 24);
      row = 0;
      /* inner loop for each row in font */
      while (row < 24)
         {
         fntp = (char *) fnt + fntofst + (row*12);
	 col = 0;
         /* inner loop for each column in font */
	 while (col < 12)
	    {
	    /* txtindx*15 to allow 3 dots between characters */
	    /* 20 pixel margin at top of PPM */
            picofst = ((row+20) * picrowsz) + (txtindx*15*3);
	    if (*fntp == '*')
	       {
	       picp = (unsigned char *) pic + picofst + (col*3) + 120;
	       *picp++ = ekisspwr(kk,6);     /* 0-63 color depth */
	       *picp++ = ekisspwr(kk,6);     /* 0-63 color depth */
	       *picp++ = ekisspwr(kk,6);     /* 0-63 color depth */
	       } /* if asterisk in font */
	    col++;
	    fntp++;
	    } /* for each column in row */
	 row++;
	 } /* for each row in font */
      txtindx++;
      } /* for each starting column in text */
   /* write the PPM map to stdout */
   putblk(pic,picsz);
   /* free allocated memory in heap */
   free(fnt);
   free(txt);
   free(pic);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
