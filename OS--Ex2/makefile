CC = gcc
FLAGS= -Wall -Werror -g

all: Main
run: main
	./$^
Main: main.c
	$(CC)  main.c -o main

clean:
	rm main
.PHONY: all run