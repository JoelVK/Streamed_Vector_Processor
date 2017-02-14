all: complementer incrementer adder

complementer: complementer.c
	gcc -Wall complementer.c -o complementer

incrementer: incrementer.c 
	gcc -Wall incrementer.c -o incrementer

adder: adder.c
	gcc -Wall adder.c -o adder
