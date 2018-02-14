#
#Project: IFJ 17 FIT BUT
#Info: Makefile
#Team: Jan Gula, Duong Nguyen Hoang, Martin Nizner 
#

CFLAGS= -pedantic -Wall -Wextra -std=gnu99 
RM=rm -f
CC=gcc
SOURCE_FILES= $(wildcard *.c)
OBJECTS = $(SOURCE_FILES:.c=.o)
PROGRAMS = ifj

all: ifj

ifj: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) *.o $(PROGRAMS)