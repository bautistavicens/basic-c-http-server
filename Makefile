
CC=gcc-4.1

CFLAGS = -Wall -O3 -std=gnu89 -pedantic-errors
HEADERS = -I./

all : clean server


server: serv_client.h
	$(CC) $(CFLAGS) -Wall -o server server.c serv_client.c -pthread $(HEADERS)
	
clean: 
	rm -rf *o server



