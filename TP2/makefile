CC=gcc
CFLAGS = -Wall -Werror
EXEC = huffman
DEBUG = gdb
FILENAME = bienvenue.txt


.PHONY: clean

default: TP2.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC)

install: TP2.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC)

#kinda cursed
run: 
	./$(EXEC) $(FILENAME)

#very cursed
debug:
	$(DEBUG) --args ./$(EXEC) $(FILENAME)

clean: 
	rm $(EXEC)
