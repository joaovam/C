#makefile

all: average.o main.o exec.out

average.o:
	gcc -c average.c

main.o:
	gcc -c main.c

exec.out:
	gcc average.o main.o -o exec.out

clean:
	rm -rf *.o *.out