.PHONY: all run clean
CC = gcc
FLAGS= 
HEADERS = 
all: server client main

server: server.o
	$(CC) $< -o server -lpthread
client: client.o
	$(CC) $< -o client
main: main.o
	$(CC) $< -o main
%.o: %.c 
	$(CC) -c $< -o $@

clean:
	rm -f *.o server client main
