all: operations.o cConv.o exec.out

operations.o:
	gcc -c operations.c
cConv.o:
	gcc -c cConv.c

exec.out:
	gcc operations.o cConv.o -o cconv
clean:
	rm -rf *.o *.out