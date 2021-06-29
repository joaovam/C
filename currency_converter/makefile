all: operations.o cConv.o exec.out clean_o

operations.o:
	gcc -c operations.c
cConv.o:
	gcc -c cConv.c

exec.out:
	gcc operations.o cConv.o -o cconv.out
	
	
clean:clean_o clean_out
	
clean_o:
	rm -rf *.o
clean_out:
	rm -rf *.out