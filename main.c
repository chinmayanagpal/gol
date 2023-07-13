#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include <stdio.h>
#include "files.h"


#define WIDTH 640
#define HEIGHT 480

app App;

void init_SDL()
{
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to init video: %s", SDL_GetError());
	}
	// SDL_CreateWindow returns a pointer to an SDL_Window

	App.window = SDL_CreateWindow("Game of Life",
					    SDL_WINDOWPOS_UNDEFINED,
					    SDL_WINDOWPOS_UNDEFINED,
					    WIDTH, HEIGHT,
					    SDL_WINDOW_SHOWN);

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
	App.texture = IMG_LoadTexture(App.renderer, "texture.png");

	if (TTF_Init() < 0) {
		printf("Failed to init TTF: %s", TTF_GetError());
	}

	App.font = TTF_OpenFont("monogram.ttf", 24);

	if (!App.font) {
		printf("Font not found!");
	}
}

void init_state(game_state *stateptr) {
	stateptr->frame.x = 50;
	stateptr->frame.y = 40;
	stateptr->frame.w = 390;
	stateptr->frame.h = 390;
	stateptr->cell_size = 12;
	stateptr->running = true;
	stateptr->paused = true;
	stateptr->board = NULL;
	stateptr->period = 8; 
	stateptr->gen = 0; 
        stateptr->origin.x = 0;
	stateptr->origin.y = 0;

}
void render_bg()
{
	for (int x = 0; x < WIDTH; x += 64) {
		for (int y = 0; y < HEIGHT; y += 64) {
			SDL_Rect srcrect= {180, 252, 16, 16};
			SDL_Rect dstrect = {x, y, 64, 64};

			// deal with corners and edges
			if (x == 0)
				srcrect.x -= 16 + 2; // the location of
			// the edge or
			// corner texture
			else if (x >= WIDTH - 64 && x < WIDTH)
			{
				srcrect.x += 16 + 2;
				dstrect.x = WIDTH - 64;
			}
			if (y == 0)
				srcrect.y -= 16 + 2;
			else if (y >= HEIGHT - 64 && y < HEIGHT)
			{
				srcrect.y += 16 + 2;
				dstrect.y = HEIGHT - 64;
			}

			SDL_RenderCopy(App.renderer,
				       App.texture, &srcrect, &dstrect);
		}
	}

}

void render_grid(SDL_Rect frame, int cell_size)
{
	SDL_SetRenderDrawColor(App.renderer, 181, 143, 104,
			       SDL_ALPHA_OPAQUE);
	int x = frame.x + frame.w / 2;
	int y = frame.y + frame.h / 2;
	while (x >= frame.x + cell_size)
		x -= cell_size;
	while (y >= frame.y + cell_size)
		y -= cell_size;
	for (; x <= frame.x + frame.w; x += cell_size) {
		SDL_RenderDrawLine(App.renderer, x, frame.y, x, frame.y + frame.h);
	}
	for (; y <= frame.y + frame.h; y += cell_size) {
		SDL_RenderDrawLine(App.renderer, frame.x, y, frame.x + frame.w, y);
	}
}

void render_frame (SDL_Rect frame)
{
	for (int x = 0; x < frame.w; x += 64) {
		SDL_Rect srcrect= {72, 151, 16, 2};
		SDL_Rect dstrect= {frame.x + x - 4, frame.y - 4, 64, 8};
		// edge case (to avoid going over frame boundary)
		// as well as to change the sprites drawn
		if (x >= frame.w - 64 && x < frame.w)
		{
			dstrect.x = frame.x + frame.h - 64 - 4;
		}
                SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.y += frame.h;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
	}
	for (int y = 0; y < frame.h; y += 64) {
		SDL_Rect srcrect= {61, 180, 2, 16};
		SDL_Rect dstrect= {frame.x - 4, frame.y + y, 8, 64};
		// edge case (to avoid going over frame boundary)
		if (y >= frame.h - 64 && y < frame.h)
		{
			dstrect.y = frame.y + frame.h - 64;
		}
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.x += frame.w;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
	}
	{
		SDL_Rect srcrect = {54, 150, 16, 5};
		SDL_Rect dstrect = {frame.x - 26, frame.y - 8, 64, 20};
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.y += frame.h;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
        }
	{
		SDL_Rect srcrect = {90, 150, 16, 5};
		SDL_Rect dstrect = {frame.x + frame.w - 64 + 26, frame.y - 8, 64, 20};
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.y += frame.h;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
        }
	{
		SDL_Rect srcrect = {60, 162, 4, 16};
		SDL_Rect dstrect = {frame.x - 8, frame.y - 26, 16, 64};
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.x += frame.w;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
        }
	{
		SDL_Rect srcrect = {60, 198, 4, 16};
		SDL_Rect dstrect = {frame.x - 8, frame.y + frame.h - 64 + 26, 16, 64};
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
		dstrect.x += frame.w;
		SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
        }

}

void render_spmat (game_state state)
{
	spmat* it = state.board;
	while (it) {
		SDL_Rect dstrect = {state.frame.x + (it->val.x - state.origin.x) * state.cell_size +  state.frame.w/2,
			state.frame.y + (it->val.y -  state.origin.y) * state.cell_size +  state.frame.h/2,
			state.cell_size,
			state.cell_size};
		SDL_Rect srcrect;
		if (dstrect.x > state.frame.x
		    && dstrect.y > state.frame.y
		    && dstrect.x + dstrect.w < state.frame.x + state.frame.w
		    && dstrect.y + dstrect.h < state.frame.y + state.frame.h) {
			srcrect.x = 109;
			srcrect.y = 397;
			srcrect.w = 14;
			srcrect.h = 14;
			SDL_RenderCopy(App.renderer, App.texture, &srcrect, &dstrect);
                } else if (dstrect.x > state.frame.x - state.cell_size
			|| dstrect.y > state.frame.y - state.cell_size
			   || dstrect.x + dstrect.w < state.frame.x + state.frame.w + state.cell_size
			   || dstrect.y + dstrect.h < state.frame.y + state.frame.h + state.cell_size) {
			SDL_Rect ndstrect;
			SDL_IntersectRect(&state.frame, &dstrect, &ndstrect);
                        if (dstrect.x > state.frame.x - state.cell_size) {
				srcrect.w = ((float) 14 * (ndstrect.w - 1)) / state.cell_size + 1;
				srcrect.x = 109 + 14 - srcrect.w;
                        }
                        if (dstrect.y > state.frame.y - state.cell_size) {
				srcrect.h = (14 * (ndstrect.h - 1)) / state.cell_size + 1;
                                srcrect.y = 397 + 14 - srcrect.h;
                        }
                        if (dstrect.x + dstrect.w >
			    state.frame.x + state.frame.w - state.cell_size) {
				srcrect.x = 109;
				srcrect.w = (14 * (ndstrect.w - 1)) / state.cell_size + 1;
                        }
                        if (dstrect.y + dstrect.h > state.frame.y + state.frame.h - state.cell_size) {
				srcrect.y = 397;
				srcrect.h = (14 * (ndstrect.h - 1)) / state.cell_size + 1;
                        }
			SDL_RenderCopy(App.renderer, App.texture, &srcrect, &ndstrect);
                }
                it = it->next;
	}
}

void render_text(game_state state)
{
	SDL_Color col = { 255, 255, 255 };
	char genstr[64];
	sprintf(genstr, "Generation: %d", state.gen);
	SDL_Surface * surface = TTF_RenderText_Solid(App.font, genstr, col);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(App.renderer, surface);
	int texth;
	int textw;
	SDL_QueryTexture(texture, NULL, NULL, &textw, &texth);
	int textx = (WIDTH + state.frame.x + state.frame.w)/2 - textw/2;
	int texty = HEIGHT/2 - texth/2;
	SDL_Rect dstrect = {textx, texty, textw, texth};
	SDL_RenderCopy(App.renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void draw(game_state state)
{
	SDL_SetRenderDrawColor(App.renderer, 0, 0, 0, 0);
	SDL_RenderClear(App.renderer);
	render_bg();
	render_grid(state.frame, state.cell_size);
	render_spmat(state);
	render_frame(state.frame);
	render_text(state);
	SDL_RenderPresent(App.renderer);
}

int alive_neighbours(spmat *mat, coord pos)
{
	int n = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= +1; ++j) {
			if (!(i == 0 && j == 0))
			{
				coord pos2 = {pos.x + i, pos.y + j};
				n += spmat_at(mat, pos2);
			}
		}
	}
	return n;
}

void iterate_board(game_state* stateptr)
{
	spmat** head = &(stateptr->board);
	spmat* old_mat = *head;
	spmat* new_mat = NULL;
	spmat* area = expanded_spmat(old_mat);
	spmat* it = area;
	bool changed = false;
	while(it) {
		int n = alive_neighbours(old_mat, it->val);
		if (spmat_at(old_mat, it->val) && (n < 2 || n > 3)) {
			spmat_set(&new_mat, it->val, false);
			changed = true;
		} else if (!spmat_at(old_mat, it->val) && n == 3) {
			spmat_set(&new_mat, it->val, true);
			changed = true;
		} else {
			spmat_set(&new_mat, it->val, spmat_at(old_mat, it->val));
		}
		it = it->next;
	}
        if (!changed) {
                stateptr->paused = true;
        } else {
                stateptr->gen++;
        }
        clear_spmat(&area);
	*head = new_mat;
	clear_spmat(&old_mat);
}

void handle_key_event(SDL_Event event, game_state* stateptr)
{
	if (event.type == SDL_KEYUP) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_COMMA:
			stateptr->period++;
			break;
		case SDL_SCANCODE_PERIOD:
			stateptr->period =
				((stateptr->period > 1) ? stateptr->period - 1 : stateptr->period);
			break;
		case SDL_SCANCODE_SPACE:
			stateptr->paused = !stateptr->paused;
			break;
		case SDL_SCANCODE_MINUS:
			stateptr->cell_size = ceil(stateptr->cell_size / 1.2);
			break;
		case SDL_SCANCODE_EQUALS:
			if (stateptr->cell_size < 30)
				stateptr->cell_size = ceil(stateptr->cell_size * 1.2);
			break;
		case SDL_SCANCODE_R:
			clear_spmat(&stateptr->board);
			FILE* fp = fopen("/home/chin/sources/gol/save.life", "r");
			stateptr->board = read_life_file(fp);
			fclose(fp);
			stateptr->gen = 0;
			break;
		case SDL_SCANCODE_DOWN:
			stateptr->origin.y++;
			break;
		case SDL_SCANCODE_UP:
			stateptr->origin.y--;
			break;
		case SDL_SCANCODE_LEFT:
			stateptr->origin.x--;
			break;
		case SDL_SCANCODE_RIGHT:
			stateptr->origin.x++;
			break;
		default:
			break;
		}
	}
}

void handle_mousebutton_event(SDL_Event event, game_state *stateptr){
        coord button_pos = {event.button.x, event.button.y};
	coord origin = stateptr->origin;
	int cell_size = stateptr->cell_size;
	coord pos;
	SDL_Rect frame =
            stateptr->frame;
	if (event.button.x > frame.x && event.button.x < frame.x + frame.w
	    && event.button.y > frame.y && event.button.y) { 
		pos.x =floor(((float)button_pos.x - frame.x - frame.w / 2) / cell_size) + origin.x;
		pos.y =floor(((float)button_pos.y - frame.y - frame.h / 2) / cell_size) + origin.y;
		printf("mouse pressed: %d, %d\n", pos.x, pos.y);
		toggle_spmat(&(stateptr->board), pos);
        }
	stateptr->gen = 0;
}
void handle_event(SDL_Event event, game_state* stateptr) {
	switch (event.type) {
	case SDL_QUIT:
		stateptr->running = false;
		break;
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		handle_key_event(event, stateptr);
		break;
	case SDL_MOUSEBUTTONUP:
		handle_mousebutton_event(event, stateptr);
	default:
		break;
	}
}
int main()
{
	init_SDL();
	game_state state;
	init_state(&state);
	FILE* fp = fopen("save.life", "r");

	state.board = read_life_file(fp);
	fclose(fp);

	int loops = 0;
	while (state.running) {
		SDL_Event event;
		while(SDL_PollEvent (&event) != 0) {
			handle_event(event, &state);
		}
		draw(state);
		if (!state.paused && loops % state.period == 0 && state.board) {
			iterate_board(&state);
		}
		SDL_Delay(16);
		loops++;
        }

        clear_spmat(&state.board);
	TTF_Quit();
	SDL_Quit();
}
