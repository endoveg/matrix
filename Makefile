debug: all
mes:
	gcc -ggdb -Wall -pthread -o mes mes.c
matrix.o:
	gcc -ggdb -Wall -c -pthread matrix.c
test: matrix.o
	gcc -ggdb -Wall -pthread -o test matrix.o test.c 
