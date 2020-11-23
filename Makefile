.PHONY: run clean all

FLAGS= -g -std=c99


all: main.o hashtable.o my_hash_tool.o
	gcc main.o hashtable.o my_hash_tool.o -o main $(FLAGS)

main.o: main.c
	gcc main.c -c $(FLAGS)

hashtable.o: hashtable.c hashtable.h
	gcc hashtable.c -c $(FLAGS)

my_hash_tool.o: my_hash_tool.c my_hash_tool.h
	gcc my_hash_tool.c -c $(FLAGS)

run: 
	./main

clean:
	rm -rf *.o
	rm -rf main
