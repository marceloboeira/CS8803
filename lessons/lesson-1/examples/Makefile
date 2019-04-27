CC=gcc
CFLAGS=-c -Wall

all: linked_list rand

.PHONY: linked_list
linked_list: linked_list.c
	@$(CC) linked_list.c -o linked_list

.PHONY:rand
rand: rand.c
	@$(CC) rand.c -o rand

clean:
	@rm linked_list rand
