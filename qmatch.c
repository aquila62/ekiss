/* qmatch.c - FIFO Queue Match for ekiss  Version 1.0.0              */
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

/* The period length of ekiss is the factorial of 1024,   */
/* or 5.41e+2639.                                         */
/* This program runs until the current 1024 outputs       */
/* match the original 1024 outputs in the same sequence.  */
/* Run this program continuously for at least 24 hours    */
/* to see how many cycles it processes without matching   */
/* the original state array.                              */

/* press 'q' to exit */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <curses.h>
#include "ekiss.h"

#define STATES (1024)

#define TENMILLION (10000000.0)
#define BILLION (1000000000.0)

/****************************************************************/
/* Structure of a FIFO queue node.                              */
/* The FIFO queue is a doubly linked list.                      */
/* The FIFO queue fits between tail and head.                   */
/* A new node is enqueued at the tail of the queue.             */
/* The oldest node is dequeued from the head of the queue.      */
/* Routines used to maintain the queue are:                     */
/* qinit  - initialize a new node                               */
/* enque  - enqueue a new node                                  */
/* deque  - dequeue a node                                      */
/* rmque  - free the entire queue                               */
/****************************************************************/
typedef struct qstruct {
   struct qstruct *prev;
   struct qstruct *next;
   unsigned int outpt;
   } qfmt;

/* initialize new curses */
void initcrss()
   {
   initscr();
   cbreak();
   noecho();
   nonl();
   intrflush(stdscr,FALSE);
   nodelay(stdscr,TRUE);
   } /* initcrss */

/****************************************************************/
/* Allocate and initialized a new node in a FIFO queue.         */
/* The calling program places the new node at the tail          */
/* of the FIFO queue.                                           */
/****************************************************************/
qfmt *qinit(void)
   {
   qfmt *newnode;
   newnode = (qfmt *) malloc(sizeof(qfmt));
   if (newnode == NULL)
      {
      fprintf(stderr,"qinit: out of memory "
         "allocating newnode\n");
      exit(1);
      } /* out of memory */
   newnode->next = newnode->prev = NULL;
   newnode->outpt = 0;
   return(newnode);
   } /* qinit */

/****************************************************************/
/* Enqueue a new node at the tail of a FIFO queue.              */
/****************************************************************/
void enque(qfmt *tail, unsigned int outpt)
   {
   qfmt *newnode;
   /* Allocate memory and initialize a new node */
   newnode = (qfmt *) qinit();
   /* Place the new node at the tail of the FIFO queue */
   newnode->next = tail->next;
   tail->next = newnode;
   newnode->next->prev = newnode;
   newnode->prev = tail;
   /* The outpt parameter is the output of an ekiss random number */
   /* generator cycle */
   newnode->outpt = outpt;
   } /* enque */

/****************************************************************/
/* Dequeue the node at the head of a FIFO queue.                */
/****************************************************************/
unsigned int deque(qfmt *head)
   {
   unsigned int outpt;
   qfmt *lastnode;
   lastnode = head->prev;
   lastnode->prev->next = head;
   head->prev = lastnode->prev;
   outpt = lastnode->outpt;
   free(lastnode);
   return(outpt);
   } /* deque */

/**********************************************************************/
/* Dequeue the least recent output from the head, then enqueue a new  */
/* node at the tail of a FIFO queue with the most recent output.      */
/* Use the same node for both dequeue and enqueue.                    */
/**********************************************************************/
void deqenq(qfmt *head, qfmt *tail, unsigned int outpt)
   {
   qfmt *node;
   node = head->prev;
   head->prev = node->prev;
   node->prev->next = head;
   /* Now place the same node at the tail of the FIFO queue */
   /* with the most recent output from the RNG */
   node->next = tail->next;
   tail->next = node;
   node->next->prev = node;
   node->prev = tail;
   /* The outpt parameter is the output of the most recent */
   /* ekiss random number generator cycle */
   node->outpt = outpt;
   } /* deqenq */

/****************************************************************/
/* Free every node in a FIFO queue.                             */
/****************************************************************/
void rmque(qfmt *tail, qfmt *head)
   {
   while (tail->next != head)
      {
      deque(head);     /* empty out the FIFO queue node by node */
      } /* for each node in queue */
   free(head);
   free(tail);
   } /* rmque */

int main()
   {
   int i;                      /* cycle counter for display */
   int ch;                     /* keyboard character */
   int diff;                   /* result of comparison 0=match */
   double tally;               /* number of FIFO queue matches */
   double cycle;               /* generator cycle number */
   qfmt *orighead;             /* head of original queue */
   qfmt *origtail;             /* tail of original queue */
   qfmt *head;                 /* head of current queue */
   qfmt *tail;                 /* tail of current queue */
   qfmt *currnode;             /* outpt in current  queue */
   qfmt *orignode;             /* outpt in original queue */
   unsigned int outpt;         /* 32-bit output from an ekiss cycle */
   kissfmt *kk;                /* ekiss structure */
   kk = (kissfmt *) ekissinit();  /* initialize the ekiss structure */

   /*************************************************************/
   /* initialize the heads and tails for the two FIFO queues    */
   /* create two empty queues, original and current             */
   /*************************************************************/
   orighead = (qfmt *) qinit();
   origtail = (qfmt *) qinit();
   head = (qfmt *) qinit();
   tail = (qfmt *) qinit();
   origtail->prev = NULL;
   origtail->next = orighead;
   orighead->next = NULL;
   orighead->prev = origtail;
   tail->prev = NULL;
   tail->next = head;
   head->next = NULL;
   head->prev = tail;

   /*************************************************************/
   /* create identical original and current FIFO queues         */
   /*************************************************************/
   outpt = 0;
   i = STATES;
   while(i--)
      {
      outpt = ekiss(kk);
      enque(origtail,outpt);
      enque(tail,outpt);
      } /* for each state in original queue */

   /********************************************************/
   /* start curses mode                                    */
   /********************************************************/

   initcrss();             /* initialize curses mode */

   /********************************************************/
   /* main generator loop                                  */
   /********************************************************/
   tally = cycle = 0.0;
   i = 0;
   while (1)         /* infinite loop */
      {
      unsigned int outpt;
      cycle += 1.0;
      i++;
      outpt = ekiss(kk);
      /******************************************************/
      /* Compare the current FIFO queue to the original     */
      /* FIFO queue.                                        */
      /* dequeue from the head of the queue                 */
      /* enqueue at the tail of the queue                   */
      /******************************************************/
      deqenq(head, tail, outpt);
      diff = 0;
      currnode = head->prev;
      orignode = orighead->prev;
      /* compare from earliest output to latest output */
      /* compare from the head to the tail of the queue */
      while (currnode != tail)
         {
	 diff = currnode->outpt - orignode->outpt;
	 if (diff) break;        /* if not equal */
	 currnode = currnode->prev;
	 orignode = orignode->prev;
	 } /* for each node in current queue */
      /* if the current FIFO queue matches the original */
      /* FIFO queue, tally                              */
      if (!diff)
	 {
	 fprintf(stderr,"main: queues match at cycle "
	    "number %20.0f\n", cycle);
         tally += 1.0;
	 } /* if queues match */
      /* update display every 10 million cycles         */
      if (i >= TENMILLION)
         {
	 char str[128];
         move(5,24);
         sprintf(str,"ekiss with Bays-Durham Shuffle");
         addstr(str);
	 move(10,27);
	 sprintf(str,"%9.2f Billion", cycle / BILLION);
	 addstr(str);
         move(15,30);
         sprintf(str,"Matches %.0f", tally);
         addstr(str);
         ch = getch();
         if (ch == 'q') break;
	 i = 0;
	 } /* every ten million cycles */
      } /* for each cycle */
   endwin();           /* de-activate curses mode */
   if (!tally)
      {
      printf("Generator has not matched "
         "as of cycle %15.2f Billion\n", cycle / BILLION);
      } /* if mis-match */
   else
      {
      printf("Generator has matched on tally %15f times\n", tally);
      } /* if mis-match */
   rmque(tail,head);
   rmque(origtail,orighead);
   free(kk->state);
   free(kk);
   return(0);
   } /* main */
