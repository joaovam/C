all: strtok.o main.o exec

strtok.o:
	gcc -c strtok.c -o strtok.o
main.o:
	gcc -c testing.c -o testing.o
exec:
	gcc strtok.o testing.o -o exec.out

clean: clean.o clean.out

clean.o:
	rm -rf *.o
clean.out:
	rm -rf *.out
