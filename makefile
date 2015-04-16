CC = gcc

INCLUDES =  

CFLAGS = -g -Wall -Werror $(INCLUDES)
CXXFLAGS = -g -Wall -Werror $(INCLUDES)
LDFLAGS = -g -lm

main: julina.o main.o
main.o: julina.h main.c
julina.o: julina.h julina.c

.PHONY: clean
clean:
	rm -f *.o main

.PHONY: all
	all: clean main

