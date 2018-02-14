CLAGS=-g -O2 
PFLAGS=-lsimlib -lm
 
CC = g++
AUTHOR = xnizne00_xgulaj00
FILES = proj.cpp 

all: proj

proj: proj.cpp
	$(CC) $(CFLAGS) -o $@ proj.cpp $(PFLAGS)

run: 
		./proj 24 520 72 default.out #kravin so zistenymi zakladnymi hodnotami, vystup je ulozeny v subore default.out
		./proj 48 520 72 zvysenedojicky.out #experiment so zvysenym poctom dojiciek, vystup je ulozeny v subore zvysenedojicky.out
		./proj 48 520 144 zvysdojcak.out #experiment so zvysenym poctom dojiciek a cakarni vystup je ulozeny v subore zvysdojcak.out
		./proj 24 520 144 zvysenacakaren.out #experiment so zvysenou kapacitou cakarne, vystup je ulozeny v subore zvysenacakaren.out
