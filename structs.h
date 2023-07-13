#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct 
{
	int x;
	int y;
} coord;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture* texture;
	TTF_Font* font;
} app;

typedef struct spmat spmat;
struct spmat
{
	coord val;
	spmat *next;
};

typedef struct {
	SDL_Rect frame;
	uint cell_size;
	spmat* board;
	bool running;
	bool paused;
	int period;
	int gen;
	coord origin;
} game_state;

// sparse matrix of booleans
// presence of a coordinate in the linked list denotes a value of 1
// otherwise 0
/* returns true if coord a and coord b have the same row and col values  */
/* false otherwise */
bool coordcmp(coord a, coord b);

/* returns the value of mat at pos */
bool spmat_at(spmat* mat, coord pos);

/* toggles the value of mat at pos */
void toggle_spmat(spmat** mat, coord pos);

/* deallocates heap memory used by *mat and sets it to NULL */
void clear_spmat(spmat **mat);

/* sets *head to a at pos */
void spmat_set(spmat** head, coord pos, bool a);

void print_spmat(spmat* head);

void print_coord(coord pos);


spmat* clone_spmat(spmat* src);

spmat* expanded_spmat(spmat* src);

coord pop_spmat(spmat **mat);

/* returns coord containing maximum row and column values of mat */
coord max_spmat(spmat* mat);

/* returns coord containing minimum row and column values of mat */
coord min_spmat(spmat* mat);
