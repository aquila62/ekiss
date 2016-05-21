/* pwrball.c - Powerball lottery simulator  Version 1.0.0            */
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
#include <curses.h>
#include "ekiss.h"

/* This program is based on the powerball calculator at */

/* http://www.lottonumbers.com/                         */
/*    lotto-odds-calculator.asp?Lottery=Powerball       */

/* the pick in this program is like a quick pick */
/* one quick pick is chosen at random and played against */
/* an infinite number of drawings, or until the quit key is pressed */

/* initialize curses */
void initcrss()
   {
   initscr();
   cbreak();
   noecho();
   nonl();
   intrflush(stdscr,FALSE);
   nodelay(stdscr,TRUE);
   } /* initcrss */

int main()
   {
   int i;              /* counter for number of balls in main pool */
   int ch;             /* end of job character read from keyboard  */ 
   int indx;           /* index into pool arrays                   */
   /* these names were taken from the website reference above      */
   int enn;            /* #balls in main  pool */
   int arr;            /* #balls in main  pool drawn   */
   int emm;            /* #balls in main  pool matched */
   int tee;            /* #balls in bonus pool */
   int dee;            /* #balls in bonus pool matched */
   int *pickp,*pickq;  /* pointers to 5 picks in main pool */
   int pick[32];       /* pick array */
   int pickbonus;      /* powerball picked */
   /* total pay-outs based on number of matches */
   /* p at end means "plus powerball" */
   /* the jackpot is totfivep */
   /* next comes 5 matches and no powerball match, etc. */
   double totfivep;
   double totfive;
   double totfourp;
   double totfour;
   double totthree;
   double totthreep;
   double tottwop;
   double totonep;
   double totzerop;
   double totprize;
   /* round is the lottery round or drawing */
   /* results of pay-outs are printed every 100 thousand rounds */
   double round;              /* counter for printing results */
   double totround;           /* total rounds played */
   char pickstr[128];         /* three strings are used for */
   char totalstr[128];        /* displaying text in curses  */
   char grandstr[128];        /* numbers are converted to text */
   kissfmt *kk;               /* ekiss structure */
   char *p,*q;                /* pointers to the pools */
   char mainpool[128];
   char maindraw[128];
   char bonuspool[64];
   kk = (kissfmt *) ekissinit();   /* initialize ekiss generator */
   initcrss();                        /* initialize curses */
   enn = 69;              /* #balls in main pool         */
   arr = 5;               /* #balls drawn from main pool */
   tee = 26;              /* #balls in bonus pool        */
   /* initialize totals */
   totprize = round = totround = 0.0;
   totfivep = totfive = totfourp = totfour = 0.0;
   totthreep = totthree = tottwop = totonep = totzerop = 0.0;
   /**************************************************/
   /* pools default to zero                          */
   /**************************************************/
   p = (char *) mainpool;
   q = (char *) mainpool + 128;
   while (p < q) *p++ = '\0';
   p = (char *) bonuspool;
   q = (char *) bonuspool + 64;
   while (p < q) *p++ = '\0';
   /**************************************************/
   /* create 5 random pick in main pool              */
   /**************************************************/
   pickp = (int *) pick;
   pickq = (int *) pick + arr;
   while (pickp < pickq)
      {
      indx = ekissint(kk,enn)+1;
      p = (char *) mainpool + indx;
      if (*p == 1) continue;
      else
         {
         *p = 1;
	 *pickp++ = indx;
	 } /* add new ball */
      } /* for each ball in main pool */
   /**************************************************/
   /* create 1 random pick in bonus pool             */
   /**************************************************/
   indx = ekissint(kk,tee) + 1;
   pickbonus = indx;
   p = (char *) bonuspool + indx;
   *p = 1;
   /**************************************************/
   /* print the header and picks at top of screen    */
   /**************************************************/
   sprintf(pickstr,"Your pick: %d %d %d %d %d  "
      "powerball %d",
      pick[0], pick[1], pick[2], pick[3],
      pick[4], pickbonus);
   move(0,30);
   addstr("Powerball Lottery");
   move(3,20);
   addstr(pickstr);
   /**************************************************/
   /* infinite number of draws                       */
   /* terminate with quit key                        */
   /**************************************************/
   while (1)
      {
      /**************************************************/
      /* zero out the draw array                        */
      /**************************************************/
      p = (char *) maindraw;
      q = (char *) maindraw + enn;;
      while (p < q) *p++ = '\0';
      /**************************************************/
      /* initialize the match counters                  */
      /**************************************************/
      i = emm = dee = 0;
      while (1)
         {
         /**************************************************/
         /* draw 5 balls from main pool                    */
         /**************************************************/
         indx = ekissint(kk,enn);
	 p = (char *) maindraw + indx;
	 if (*p == 1) continue;
	 *p = 1;
	 i++;
	 p = (char *) mainpool + indx;
	 if (*p != 0)
	    {
	    emm++;
	    } /* matched ball drawn */
	 if (i >= arr) break;
	 } /* for each main ball drawn */
      /**************************************************/
      /* draw 1 balls from bonus pool                   */
      /**************************************************/
      indx = ekissint(kk,tee);
      p = (char *) bonuspool + indx;
      if (*p != 0)
         {
         dee = 1;
         } /* bonus ball matched */
      /**************************************************/
      /* add 1 to number of draws (or rounds)           */
      /**************************************************/
      round += 1.0;
      totround += 1.0;
      /**************************************************/
      /* if jackpot                                     */
      /* display jackpot totals on screen               */
      /**************************************************/
      if (emm >= arr && dee == 1)
	 {
         totprize += 1000000000.0;
	 totfivep += 1000000000.0;
	 move(6,15);
	 sprintf(totalstr,"5%c  %f M$", '+', totfivep / 1000000.0);
	 addstr(totalstr);
	 move(21,20);
	 sprintf(grandstr,"spent %.1f M$  received %f M$",
	    totround / 1000000.0, totprize / 1000000.0);
	 addstr(grandstr);
	 refresh();
	 } /* if hit jackpot */
      /**************************************************/
      /* tally the pay-outs for the prize levels        */
      /**************************************************/
      else if (emm == 5 && dee == 0)
	 {
         totprize += 1000000.0;
	 totfive  += 1000000.0;
	 }
      else if (emm == 4 && dee == 1)
	 {
         totprize += 50000.0;
	 totfourp += 50000.0;
	 }
      else if (emm == 4 && dee == 0)
	 {
         totprize += 100.0;
         totfour  += 100.0;
	 }
      else if (emm == 3 && dee == 1)
         {
         totprize  += 100.0;
         totthreep += 100.0;
         }
      else if (emm == 3 && dee == 0)
         {
         totprize += 7.0;
         totthree += 7.0;
	 }
      else if (emm == 2 && dee == 1)
         {
         totprize += 7.0;
         tottwop  += 7.0;
	 }
      else if (emm == 1 && dee == 1)
         {
         totprize += 4.0;
         totonep  += 4.0;
	 }
      else if (emm == 0 && dee == 1)
         {
         totprize += 4.0;
         totzerop += 4.0;
	 }
      /**************************************************/
      /* every 100 thousand rounds, display totals      */
      /**************************************************/
      if (round >= 100000.0)
         {
	 /******************* 5 + 1 *******************/
	 move(6,15);
	 sprintf(totalstr,"5%c  %f M$", '+', totfivep / 1000000.0);
	 addstr(totalstr);
	 /******************* 5 + 0 *******************/
	 move(6,45);
	 sprintf(totalstr,"5   %f M$", totfive / 1000000.0);
	 addstr(totalstr);
	 /******************* 4 + 1 *******************/
	 move(9,15);
	 sprintf(totalstr,"4%c  %.3f K$", '+', totfourp / 1000.0);
	 addstr(totalstr);
	 /******************* 4 + 0 *******************/
	 move(9,45);
	 sprintf(totalstr,"4   %.3f K$", totfour / 1000.0);
	 addstr(totalstr);
	 /******************* 3 + 1 *******************/
	 move(12,15);
	 sprintf(totalstr,"3%c  %.3f K$", '+', totthreep / 1000.0);
	 addstr(totalstr);
	 /******************* 3 + 0 *******************/
	 move(12,45);
	 sprintf(totalstr,"3   %.3f K$", totthree / 1000.0);
	 addstr(totalstr);
	 /******************* 2 + 1 *******************/
	 move(15,15);
	 sprintf(totalstr,"2%c  %.3f K$", '+', tottwop / 1000.0);
	 addstr(totalstr);
	 /******************* 1 + 1 *******************/
	 move(15,45);
	 sprintf(totalstr,"1%c  %.3f K$", '+', totonep / 1000.0);
	 addstr(totalstr);
	 /******************* 1 + 1 *******************/
	 move(18,15);
	 sprintf(totalstr,"PB  %.3f K$", totzerop / 1000.0);
	 addstr(totalstr);
	 /******************* grand total  *******************/
	 move(21,20);
	 sprintf(grandstr,"spent %.1f M$  received %f M$",
	    totround / 1000000.0, totprize / 1000000.0);
	 addstr(grandstr);
	 move(23,60);
	 addstr("q = quit");
	 refresh();                /* refresh screen */
	 round = 0.0;              /* reset round counter */
         /**************************************************/
         /* Test the keyboard for quit command             */
         /**************************************************/
         ch = getch();      /* test keyboard for interrupt */
         /* quit if 'escape' or 'q' */
         if (ch == 0x1b) /* ESCAPE */
            break;
         if (ch == 'q') /* quit */
            break;
         /* reset screen to blank if 'r' */
         if (ch == 'r') /* reset */
            {
            clear();
            refresh();
            } /* reset */
	 } /* print current status */
      } /* for each drawing */
   /* end of job */
   endwin();           /* de-activate curses mode */
   free(kk->state);    /* free state array */
   free(kk);           /* free ekiss structure */
   return(0);          /* normal eoj */
   } /* main */
