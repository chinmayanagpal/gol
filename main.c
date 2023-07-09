#include <SDL2/SDL.h>
#include <stdio.h>
#include "structs.h"

struct app {
				SDL_Window *window;
				SDL_Renderer *renderer;
} App;

void init_SDL(struct app App, struct coord size)
{
				if (SDL_Init(SDL_INIT_VIDEO) < 0)
				{
								printf("Failed to init video: %s", SDL_GetError());
				}
				// SDL_CreateWindow returns a pointer to an SDL_Window

				App.window = SDL_CreateWindow("Game of Life",
																															SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
																															size.row * 10, size.column * 10, 0);

				// similar to window (see above comment)
				App.renderer = SDL_CreateRenderer (App.window, -1, SDL_RENDERER_ACCELERATED);
}

void draw_sparsematrix(struct sparsematrix mat, struct coord min, struct coord max, int cell_size)
{
				struct sparsematrix *it = &mat;
				while (it)
				{
								SDL_Rect rect = {it->value.row * cell_size, it->value.column *  cell_size,
																									cell_size, cell_size};
								SDL_SetRenderDrawColor(App.renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
								SDL_RenderFillRect(App.renderer, &rect);
								it = it->next;
				}
}

int main()
{
				struct coord size;
				size.row = 100;
				size.column = 100;
				init_SDL(App, size);
				struct sparsematrix mat;
				mat.next = NULL;
				struct coord cell = {5, 5};
				mat.value = cell;
				struct coord max = {100, 100};
				int cell_size = 10;
				draw_sparsematrix(mat, dimensions, cell_size);
				while (true) {
								printf("");
				}
}
