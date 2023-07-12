main: main.c structs.c files.c
	gcc -g structs.c main.c files.c -lSDL2 -o main
