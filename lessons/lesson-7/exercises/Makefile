CC=gcc
CFLAGS=-c -Wall -lpthread
RS=rustc

all: priority_readers_writers socket_server_rust socket_client_rust socket_server_c socket_client_c

.PHONY: priority_readers_writers
priority_readers_writers: priority_readers_writers.c
	@$(CC) priority_readers_writers.c -o priority_readers_writers

.PHONY: socket_server_rust
socket_server_rust: socket_server.rs
	@$(RS) socket_server.rs -o socket_server_rust

.PHONY: socket_client_rust
socket_client_rust: socket_client.rs
	@$(RS) socket_client.rs -o socket_client_rust

.PHONY: socket_server_c
socket_server_c: socket_server.c
	@$(CC) socket_server.c -o socket_server_c

.PHONY: socket_client_c
socket_client_c: socket_client.c
	@$(CC) socket_client.c -o socket_client_c

.PHONY: clean
clean:
	@rm priority_readers_writers socket_server_rust socket_client_rust socket_server_c socket_client_c
