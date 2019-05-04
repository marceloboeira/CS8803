CC=gcc
CFLAGS=-c -Wall -lpthread

all: simple_thread thread_with_params thread_with_correct_params

.PHONY: simple_thread
simple_thread: simple_thread.c
	@$(CC) simple_thread.c -o simple_thread

.PHONY: thread_with_params
thread_with_params: thread_with_params.c
	@$(CC) thread_with_params.c -o thread_with_params

.PHONY: thread_with_correct_params
thread_with_correct_params: thread_with_correct_params.c
	@$(CC) thread_with_correct_params.c -o thread_with_correct_params

clean:
	@rm simple_thread thread_with_params thread_with_correct_params
