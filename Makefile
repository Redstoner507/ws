#==========================================
#    Makefile: makefile for ws 2.0
#	Copyright 2024
#                 Stoll-Geyer Yann
#	Last Modified: 17/11/2024
#==========================================

CC=gcc
CFLAGS=-O -Wall

all: ws

ws: ws.c ws.h
	$(CC) $(CFLAGS) -o ws ws.c -lncurses

clean:
	rm -f ws

distclean: clean
