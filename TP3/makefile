CC=gcc
CFLAGS = -Wall -Werror
EXEC = main


.PHONY: clean

default:  
	part2

part1: part1.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC) -lm

part2: part2.c
	$(CC) $(CFLAGS) -O2 $? -o $(EXEC) -lm


clean: 
	rm $(EXEC)
