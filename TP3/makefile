CC=gcc
CFLAGS = -Wall -Werror 
EXEC = main


.PHONY: clean

default: part2.c 
	$(CC) $(CFLAGS) -g $? -o $(EXEC) -lm

install: part2.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC) -lm

run:
	./$(EXEC)

clean: 
	rm $(EXEC)
