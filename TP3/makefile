CC=gcc
CFLAGS = -Wall -Werror 
EXEC = main


.PHONY: clean

default: main.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC) -lm

install: main.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC) -lm

run:
	./$(EXEC)

clean: 
	rm $(EXEC)
