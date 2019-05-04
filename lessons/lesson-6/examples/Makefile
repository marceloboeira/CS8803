CC=gcc
CFLAGS=-c -Wall -lpthread

all: simple_thread thread_with_params thread_with_correct_params producer_consumer

.PHONY: simple_thread
simple_thread: simple_thread.c
	@$(CC) simple_thread.c -o simple_thread

.PHONY: thread_with_params
thread_with_params: thread_with_params.c
	@$(CC) thread_with_params.c -o thread_with_params

.PHONY: thread_with_correct_params
thread_with_correct_params: thread_with_correct_params.c
	@$(CC) thread_with_correct_params.c -o thread_with_correct_params

.PHONY: producer_consumer
producer_consumer: producer_consumer.c
	@$(CC) producer_consumer.c -o producer_consumer

clean:
	@rm simple_thread thread_with_params thread_with_correct_params producer_consumer
