all: shared_mem shared_mutex shared_cond int_32 main exec
shared_mem:
	gcc -c shared_mem.c -o shared_mem.o

shared_cond:
	gcc -c shared_cond.c -o shared_cond.o
shared_mutex:
	gcc -c shared_mutex.c -o shared_mutex.o

int_32:
	gcc -c shared_int32.c -o shared_int32.o
main:
	gcc -c main.c -o main.o


exec: 
	gcc shared_mem.o shared_mutex.o shared_cond.o shared_int32.o main.o -o exec.out -lpthread -lrt