CC=gcc
CFLAGS=-c -Wall -lpthread

all: priority_readers_writers

.PHONY: priority_readers_writers
priority_readers_writers: priority_readers_writers.c
	@$(CC) priority_readers_writers.c -o priority_readers_writers

clean:
	@rm priority_readers_writers
