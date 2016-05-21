/*
ranwlk.c Version 1.0.0. Random walk with random input
Copyright (C) 2016    aquila62 at github.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to:

	Free Software Foundation, Inc.
	59 Temple Place - Suite 330
	Boston, MA  02111-1307, USA.
*/

/* this program performs a random walk in X Windows            */
/* based on random input                                       */
/* etausgen.c produces the input data through a pipe           */
/* use the tstwlk.sh script to test this program with etausgen */
/* Press any key to quit                                       */

/* to define the escape key */
#define XK_MISCELLANY 1
#define XK_LATIN1 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <assert.h>

#define NIL (0)

/* X Windows structure */
typedef struct xxstruct {
   int whiteColor,blackColor;
   unsigned long blue;
   unsigned long red;
   int rootwh;      /* root window height */
   int rootww;      /* root window width  */
   int dpyhght;     /* display window height */
   int dpywdth;     /* display window width  */
   Display *dpy;
   Window w;
   GC gc;
   Font fontid;
   Screen *dfltscr;
   Colormap cmap;
   XColor scrdef,exctdef;
   char title[256];
   int runflg,resetsw;
   int *newwave;
   int middlerow;
   int col;
   int row;
   } xxfmt;

/* X Windows code is based on: */
/* http://tronche.lri.fr:8000/gui/x/xlib-tutorial/2nd-program-anatomy.html */

/* wait for keyboard input character */
void getkey(xxfmt *xx)
   {
   int msk;
   int symbol;
   int XCheckMaskEvent();
   XEvent e;
   XKeyEvent *k;

   msk = KeyPressMask|ExposureMask;

   XSelectInput(xx->dpy, xx->w, msk);

   while (XCheckMaskEvent(xx->dpy, msk, &e))
      {
      if (e.type == KeyPress)
         {
         k = (XKeyEvent *) &e;
         symbol = XLookupKeysym(k,0);
         xx->resetsw = 0;
         if (symbol == XK_Escape || symbol == 'q')
            {
            xx->runflg = 0;
            } /* if quit */
         else if (symbol == 'r')
            {
            xx->resetsw = 1;
            } /* reset */
         } /* if keypress event */
      else if (e.type == Expose)
         {
	 XClearWindow(xx->dpy,xx->w);
         } /* if expose event */
      } /* if event received */
   } /* getkey */

/* test for keyboard input character */
void ifkey(xxfmt *xx)
   {
   int msk;
   int XCheckMaskEvent();
   XEvent e;

   msk = KeyPressMask|ExposureMask;

   XSelectInput(xx->dpy, xx->w, msk);

   xx->runflg = 1;

   while (XCheckMaskEvent(xx->dpy, msk, &e))
      {
      if (e.type == KeyPress)
         {
	 exit(0);
         } /* if keypress event */
      else if (e.type == Expose)
         {
	 XClearWindow(xx->dpy,xx->w);
	 return;
         } /* if expose event */
      } /* if event received */
   } /* ifkey */

/* initialize X Windows */
void initx(xxfmt *xx)
   {
   int rslt;

   xx->dpy = XOpenDisplay(NIL);

   if (xx->dpy == NULL)
      {
      fprintf(stderr,"X Windows failure\n");
      exit(1);
      } /* if X Windows is not active */

   assert(xx->dpy);

   /* get dimensions of root window */
   xx->rootww = XDisplayWidth(xx->dpy,0);
   xx->rootwh = XDisplayHeight(xx->dpy,0);

   /* make display window smaller than root window */
   /* allow for menu bar on top */
   xx->dpywdth = xx->rootww -  80;
   xx->dpyhght = xx->rootwh - 100;

   xx->whiteColor = WhitePixel(xx->dpy, DefaultScreen(xx->dpy));
   xx->blackColor = BlackPixel(xx->dpy, DefaultScreen(xx->dpy));

   xx->w = XCreateSimpleWindow(xx->dpy,
      DefaultRootWindow(xx->dpy),
      0, 0, 
      xx->dpywdth, xx->dpyhght,
      0, xx->whiteColor,
      xx->whiteColor);

   XSelectInput(xx->dpy, xx->w, StructureNotifyMask);

   XMapWindow(xx->dpy, xx->w);

   xx->gc = XCreateGC(xx->dpy, xx->w, 0, NIL);

   xx->fontid = (Font) XLoadFont(xx->dpy,"12x24");

   XSetFont(xx->dpy,xx->gc,xx->fontid);

   XSetBackground(xx->dpy, xx->gc, xx->whiteColor);
   XSetForeground(xx->dpy, xx->gc, xx->blackColor);

   xx->dfltscr = XDefaultScreenOfDisplay(xx->dpy);
   if (xx->dfltscr == NULL)
      {
      fprintf(stderr,"XDefaultScreenOfDisplay failed\n");
      perror("XDefaultScreenOfDisplay failed");
      exit(1);
      } /* if error */

   xx->cmap = XDefaultColormapOfScreen(xx->dfltscr);

   rslt = XAllocNamedColor(xx->dpy,xx->cmap,"blue",
      &xx->scrdef,&xx->exctdef);
   if (rslt < 0)
      {
      fprintf(stderr,"XAllocNamedColor failed\n");
      perror("XAllocNamedColor failed");
      exit(1);
      } /* if error */
   xx->blue = xx->scrdef.pixel;

   rslt = XAllocNamedColor(xx->dpy,xx->cmap,"red",
      &xx->scrdef,&xx->exctdef);
   if (rslt < 0)
      {
      fprintf(stderr,"XAllocNamedColor failed\n");
      perror("XAllocNamedColor failed");
      exit(1);
      } /* if error */
   xx->red = xx->scrdef.pixel;

   XSetWindowBorderWidth(xx->dpy, xx->w, 40);

   sprintf(xx->title,"Random Walk");
   XStoreName(xx->dpy,xx->w,xx->title);
   XSetIconName(xx->dpy,xx->w,xx->title);

   while(1)
      {
      XEvent e;
      XNextEvent(xx->dpy, &e);
      if (e.type == MapNotify) break;
      } /* wait for window initialization */

   } /* initx */

/* read one ASCII byte from stdin */
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
      } /* read err */
   return(buf[0]);
   } /* getbyte */

/* read one ASCII bit (0 or 1) from stdin */
/* bypass white space */
/* abort if end of file */
int getbit(void)
   {
   int ch;
   while (1)
      {
      ch = getbyte();
      if (ch == EOF)
         {
         fprintf(stderr,"getbit: end of input file\n");
         exit(1);
         } /* small file */
      if (ch == '0' || ch == '1') break;
      } /* bypass white space */
   return(ch);
   } /* getbit */

/* shift all other pixels one pixel to the left */
/* add one random pixel to end of the random walk */
/* return end of file flag if end of file */
/* otherwise return zero */
int walk(xxfmt *xx)
   {
   int col;         /* column coordinate */
   int row;         /* row    coordinate */
   int ch;          /* input character */
   int *p,*q,*r;    /* pointers to the random walk array */
   /* shift random walk pixels one pixel to the left */
   col = 0;
   p = (int *) xx->newwave;
   q = (int *) xx->newwave + xx->dpywdth - 1;
   r = (int *) xx->newwave + 1;
   while (p < q)
      {
      *p = *r;
      /* erase pixel + 1 */
      if (*r != xx->middlerow)    /* do not erase the middle row */
         {
         XSetForeground(xx->dpy, xx->gc, xx->whiteColor);
         XDrawPoint(xx->dpy, xx->w, xx->gc, col+1, *r);
         } /* if not the red line */
      /* copy to pixel + 0 */
      if (*p != xx->middlerow)    /* do not write over the middle row */
         {
         XSetForeground(xx->dpy, xx->gc, xx->blackColor);
         XDrawPoint(xx->dpy, xx->w, xx->gc, col, *p);
         } /* if not the red line */
      /* point to next pixel, and repeat loop */
      col++;
      p++;
      r++;
      } /* for each pixel in the wave */
   ch = getbit();    /* read one random bit from stdin */
   /* this program is sometimes used to read a finite */
   /* irrational number binary expansion */
   /* therefore it sometimes reads to end of file */
   if (ch == EOF) return(EOF);   /* if end of file, go to end of job */
   /* if input bit is '1', raise random walk one pixel */
   if (ch == '1')
      {
      row = *p + 1;
      if (row > xx->dpyhght) row = xx->dpyhght;
      *p = row;
      } /* if input bit == '1' */
   /* if input bit is '0', lower random walk one pixel */
   else
      {
      row = *p - 1;
      if (row < 0) row = 0;
      *p = row;
      } /* else input bit == '0' */
   /* write pixel at end of random walk */
   XSetForeground(xx->dpy, xx->gc, xx->blackColor);
   XDrawPoint(xx->dpy, xx->w, xx->gc, col, row);
   /* re-write middle line in red */
   XSetForeground(xx->dpy, xx->gc, xx->red);
   XDrawLine(xx->dpy, xx->w, xx->gc,
      0, xx->middlerow,
      xx->dpywdth, xx->middlerow);
   return(0);        /* return not-end-of-file */
   } /* walk */

int main()
   {
   int *p,*q;      /* pointer to random walk array */
   int rslt;       /* return code from walk() */ 
   xxfmt *xx;      /* declare X Windows structure */
   /* allocate memory for X Windows structure */
   xx = (xxfmt *) malloc(sizeof(xxfmt));
   if (xx == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx\n");
      exit(1);
      } /* out of mem */
   initx(xx);         /* initialize X Windows */
   /* allocate memory for random walk array */
   xx->newwave = (int *) malloc(xx->dpywdth + 100);
   if (xx->newwave == NULL)
      {
      fprintf(stderr,"main: out of memory "
         "allocating xx->newwavemtx\n");
      exit(1);
      } /* out of mem */
   /* set y coordinate for the middle row line */
   xx->middlerow = xx->dpyhght >> 1;
   /*************************************/
   /* paint the middle row line         */
   /*************************************/
   p = (int *) xx->newwave;
   q = (int *) xx->newwave + xx->dpywdth;
   while (p < q) *p++ = xx->middlerow;
   /*************************************/
   xx->runflg = 1;      /* set the run flag to run */
   while (xx->runflg)   /* random walk until quit or eof */
      {
      rslt = walk(xx);  /* walk one pixel */
      if (rslt == EOF) break;     /* if eof, go to end of job */
      ifkey(xx);           /* if keyboard interrupt test for 'q' */
      } /* for each pixel in random walk */
   /* end of job: free memory */
   XDestroyWindow(xx->dpy, xx->w);     /* delete current window */
   XCloseDisplay(xx->dpy);             /* stop X Windows */
   free(xx->newwave);                  /* free random walk array */
   free(xx);                           /* free X Windows structure */
   return(0);                          /* normal end of job */
   } /* main */
