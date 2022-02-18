CC=gcc
CFLAGS = -Wall -Werror
EXEC = jpeg


.PHONY: clean

default: main.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC)

install: main.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC)

run:
	./$(EXEC)

clean: 
	rm $(EXEC)
