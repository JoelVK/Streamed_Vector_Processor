all: complementer incrementer adder

complementer: complementer.c
	gcc -Wall -g complementer.c -o complementer

incrementer: incrementer.c 
	gcc -Wall -g incrementer.c -o incrementer

adder: adder.c
	gcc -Wall -g adder.c -o adder
