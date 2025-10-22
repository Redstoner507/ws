#==========================================
#    Makefile: makefile for ws 2.5
#	Copyright 2025
#                 Stoll-Geyer Yann
#	Last Modified: 22/10/2025
#==========================================

CC=gcc
CFLAGS= -O -Wall
LIBS = -lncurses

all: ws

ws: ws.c ws.h
	$(CC) $(CFLAGS) -o ws ws.c $(LIBS)

clean:
	rm -f ws

distclean: clean
