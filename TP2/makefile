CC=gcc
CFLAGS = -Wall -Werror
EXEC = huffman


.PHONY: clean

default: TP2.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC)

install: TP2.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC)


clean: 
	rm $(EXEC)
