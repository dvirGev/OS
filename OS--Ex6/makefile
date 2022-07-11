.PHONY: all run clean
CC = gcc
GG = g++
FLAGS= -pthread
HEADERS = 
all: main1 client1 guard singleton reactor clientr

main1: main1.o
	$(CC) $< -o main1 $(FLAGS)

client1: client1.o
	$(CC) $< -o client1 

guard: 
	$(GG) guard.cpp -o guard $(FLAGS)

singleton: 
	$(GG) singleton.cpp -o singleton $(FLAGS)

reactor: reactor.o
	$(CC) $<  -o reactor $(FLAGS)

clientr: pollclient.o
	$(CC) $<  -o clientr $(FLAGS)
test: autoClient test.o
	$(CC) test.o -o test
autoClient: autoClient.o
	$(CC) $< -o autoClient
%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o main1 client1 guard singleton reactor clientr