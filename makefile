main: main.c structs.c
	gcc -g structs.c main.c -lSDL2 -o main
