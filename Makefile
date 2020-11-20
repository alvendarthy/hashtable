all: main.o hashtable.o
	gcc main.o hashtable.o -o main

main.o: main.c
	gcc main.c -c

hashtable.o: hashtable.c hashtable.h
	gcc hashtable.c -c

clean:
	rm -rf *.o
	rm -rf main
