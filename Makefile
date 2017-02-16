all: complementer

complementer: complementer.c
	gcc -Wall -g complementer.c -o complementer
