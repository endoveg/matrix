mes:
	gcc -ggdb -Wall -pthread -o mes mes.c
matrix.o:
	gcc -pthread -ggdb -Wall -c  matrix.c
test: matrix.o
	gcc -ggdb -Wall -pthread -o test matrix.o test.c 
test2: matrix.o
	gcc -ggdb -Wall -pthread -o test2 matrix.o test2.c
test3: matrix.o
	gcc -ggdb -Wall -pthread -o test3 matrix.o test3.c
server.o:
	gcc -pthread -ggdb -Wall  -c server.c
server: matrix.o server.o
	gcc -pthread -ggdb -Wall -o server matrix.o server.o
client.o:
	gcc -pthread -ggdb -Wall -c client.c
client: matrix.o client.o
	gcc -pthread -ggdb -Wall -o client client.o matrix.o
clean: 
	ipcrm -Q 0x2a010e42
	ipcrm -M 0x2a010e42


