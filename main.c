#include <SDL2/SDL.h>
#include <stdio.h>
#include "structs.h"

struct app {
	SDL_Window *window;
	SDL_Renderer *renderer;
} App;

void init_SDL(struct app App, struct coord size, int cell_size)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to init video: %s", SDL_GetError());
	}
	// SDL_CreateWindow returns a pointer to an SDL_Window

	App.window = SDL_CreateWindow("Game of Life",
				      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				      size.row * cell_size, size.col * cell_size,
				      0);

	// similar to window (see above comment)
	App.renderer = SDL_CreateRenderer (App.window, -1, SDL_RENDERER_ACCELERATED);
}

void draw_sparsematrix (struct sparsematrix *mat, int cell_size)
{
	SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(App.renderer);
	struct sparsematrix *it = mat;
	while (it) {
		SDL_Rect rect = {it->val.row * cell_size,
			it->val.row * cell_size,
			cell_size,
			cell_size};
		SDL_SetRenderDrawColor(App.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(App.renderer, &rect);
		printf("%d, %d\n", it->val.row, it->val.col);
		it = it->next;
	}
	SDL_RenderPresent(App.renderer);
}


int main()
{
	struct coord size = {50, 50};
	int cell_size = 10;
	init_SDL(App, size, cell_size);
	struct sparsematrix *mat = NULL;
	clear_sparsematrix(&mat);
}
