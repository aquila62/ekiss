/* ekissint.c - ekiss random integer Version 1.0.0                   */
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

/* this subroutine generates a random integer from zero */
/* up to a limit                                        */
/* the limit may be positive or negative                */
/* For performance reasons, the input parameter limit   */
/* is not validated                                     */
/* See ekisspwr.c for generating an unsigned integer    */
/* of 1-32 bits.                                        */

#include <math.h>
#include "ekiss.h"

#define MAXINT 4294967296.0

int ekissint(kissfmt *kk, int limit)
   {
   int intgr;                     /* output signed integer */
   double num;                    /* uniform random number 0-1 */
   /* generate uniform random number 0-1 */
   num = (double) ekiss(kk) / MAXINT;   /* generate random number 0-1 */
   /* multiply num by the limit and truncate */ 
   /* this produces an integer from zero up to the limit */
   intgr = (int) floor(num * (double) limit);
   return(intgr);           /* return the signed integer */
   } /* ekissint */
