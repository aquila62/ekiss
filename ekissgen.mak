#  ekissgen.mak - Compile ekissgen.c Version 1.0.0
#  Copyright (C) 2016 aquila62 at github.com

#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License as
#  published by the Free Software Foundation; either version 2 of
#  the License, or (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to:

   #  Free Software Foundation, Inc.
   #  59 Temple Place - Suite 330
   #  Boston, MA 02111-1307, USA.

# this program feeds ranwlk.c
# use tstwlk.sh to test this program with ranwlk.c

OBJ=ekissgen.o \
	ekissinit.o \
	ekiss.o \
	ekissbit.o

CC=gcc

CFLAGS=-c -Wall -O2

LDFLAGS=

ekissgen:			$(OBJ)
		$(CC) -Wall -O2 $(OBJ) -o ekissgen $(LDFLAGS)

ekissgen.o:			ekissgen.c
		$(CC) $(CFLAGS) ekissgen.c

ekissinit.o:			ekissinit.c
		$(CC) $(CFLAGS) ekissinit.c

ekiss.o:			ekiss.c
		$(CC) $(CFLAGS) ekiss.c

ekissbit.o:			ekissbit.c
		$(CC) $(CFLAGS) ekissbit.c

clean:
		rm -f $(OBJ) ekissgen
