CC=gcc
CFLAGS = -Wall -Werror
EXEC = huffman
DEBUG = gdb

.PHONY: clean

default: TP2.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC)

install: TP2.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC)

#kinda cursed
run: 
	./$(EXEC) bienvenue.txt

#very cursed
debug:
	$(DEBUG) ./$(EXEC)

clean: 
	rm $(EXEC)
