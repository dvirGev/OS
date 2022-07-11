CC = gcc
FLAGS= -Wall -Werror -g
all: main  server 
run: Main
	./$^
main: main.o client.o
	$(CC) $(FLAGS) $^ -o main
server: server.o
	$(CC) $(FLAGS) $^ -o server
%.o: %.c
	$(CC) -c $< -o $@
clean:
	rm *.o main server