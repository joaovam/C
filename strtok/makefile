all: strtok.o main.o exec clean.o

strtok.o:
	gcc -c strtok.c
main.o:
	gcc -c main.c
exec:
	gcc strtok.o main.o -o exec.out

clean: clean.o clean.out

clean.o:
	rm -rf *.o
clean.out:
	rm -rf *.out
