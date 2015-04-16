CC = gcc

INCLUDES =  

CFLAGS = -g -Wall $(INCLUDES)
CXXFLAGS = -g -Wall $(INCLUDES)
LDFLAGS = -g -lm

main: linea.o main.o
main.o: linea.h main.c
linea.o: linea.h linea.c

.PHONY: clean
clean:
	rm -f *.o main

.PHONY: all
	all: clean main

