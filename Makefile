all : calc
calc : main.c stack.c stack.h
	clang -o calc main.c stack.c
