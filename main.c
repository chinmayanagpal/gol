#include <SDL2/SDL.h>
#include <stdio.h>
#include "files.h"

struct app {
	SDL_Window *window;
	SDL_Renderer *renderer;
} App;

void init_SDL(struct coord size, int cell_size)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to init video: %s", SDL_GetError());
	}
	// SDL_CreateWindow returns a pointer to an SDL_Window

	App.window = SDL_CreateWindow("Game of Life",
				      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
				      size.row * cell_size, size.col * cell_size,
				      0);

	if (!App.window)
	{
		printf("Failed to open window: %s", SDL_GetError());
		exit(1);
	}

	// similar to window (see above comment)
	App.renderer = SDL_CreateRenderer (App.window, -1, SDL_RENDERER_ACCELERATED);

	if (!App.renderer)
	{
		printf("Failed to create renderer: %s", SDL_GetError());
		exit(1);
	}
}

void draw_spmat (struct spmat* mat, int cell_size, struct coord size)
{
	SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, 255);
	SDL_RenderClear(App.renderer);
	struct spmat* it = mat;
	while (it) {
		if (it->val.row >= 0 && it->val.col >= 0 && it->val.row < size.row && it->val.col < size.col)
		{
			SDL_Rect rect = {it->val.row * cell_size,
				it->val.col * cell_size,
				cell_size, cell_size};
			SDL_SetRenderDrawColor(App.renderer, 100, 160, 200,
					       SDL_ALPHA_OPAQUE);
			SDL_RenderFillRect(App.renderer, &rect);
		}
		it = it->next;
	}
	SDL_RenderPresent(App.renderer);
}

int alive_neighbours(struct spmat *mat, struct coord pos)
{
	int n = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= +1; ++j) {
			if (!(i == 0 && j == 0))
			{
				struct coord pos2 = {pos.row + i, pos.col + j};
				n += spmat_at(mat, pos2);
			}
		}
	}
	return n;
}

void iterate_spmat(struct spmat** head)
{
	struct spmat* old_mat = clone_spmat(*head);
	struct coord max=  max_spmat(old_mat);
	struct coord min=  min_spmat(old_mat);
	for (int i = min.row - 1; i <= max.row + 1; ++i) {
		for (int j = min.col - 1; j <= max.col + 1; ++j) {

			struct coord pos = {i, j};
			int n = alive_neighbours(old_mat, pos);

			if (spmat_at(old_mat, pos) && (n < 2 || n > 3)) {
				toggle_spmat(head, pos);
			} else if (!spmat_at(old_mat, pos) && n == 3) {
				toggle_spmat(head, pos);
			} else {
				spmat_set(head, pos, spmat_at(old_mat, pos));
			}
		}
	}
	struct spmat* this = old_mat;
	clear_spmat(&old_mat);
}

void iterate_spmat2(struct spmat** head)
{
	struct spmat* old_mat = *head;
	struct spmat* new_mat = NULL;
	struct spmat* area = expanded_spmat(old_mat);
	struct spmat* this = area;
	while(this) {
		int n = alive_neighbours(old_mat, this->val);
		if (spmat_at(old_mat, this->val) && (n < 2 || n > 3)) {
			spmat_set(&new_mat, this->val, false);
		} else if (!spmat_at(old_mat, this->val) && n == 3) {
			spmat_set(&new_mat, this->val, true);
		} else {
			spmat_set(&new_mat, this->val, spmat_at(old_mat, this->val));
		}
		this = this->next;
	}
	clear_spmat(&area);
	*head = new_mat;
	clear_spmat(&old_mat);
}

int main()
{
	struct coord size = {50, 50};
	int cell_size = 10;
	init_SDL(size, cell_size);

	FILE* fp = fopen("/home/chin/sources/gol/save.life", "r");
	struct spmat *mat = read_life_file(fp);
	fclose(fp);

	bool running = true;
	bool paused = true;
	while (running) {
		SDL_Event event;
		while(SDL_PollEvent (&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYUP:
				paused = !paused;
				break;
			default:
				break;
			}
		}
		draw_spmat(mat, cell_size, size);
		if (!paused)
			iterate_spmat2(&mat);
		SDL_Delay(250);
	}
	fp = fopen("/home/chin/sources/gol/save.life", "w");
	write_life_file(fp, mat);
	fclose(fp);
	clear_spmat(&mat);
	SDL_Quit();
}
