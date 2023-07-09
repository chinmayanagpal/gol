#include <iostream>
#include <SDL2/SDL.h>
#include <set>
#include <utility>

#define XSIZE 100
#define YSIZE 100
#define CELL_SIZE 10

bool grid[XSIZE][YSIZE];
bool new_grid[XSIZE][YSIZE];
bool printed;
bool running;
SDL_Window *window;
SDL_Renderer *renderer;

void init_grid()
{
	for (int i = 0; i < XSIZE; ++i)
	{
		for (int j = 0; j < YSIZE; ++j)
		{
			grid[i][j] = 0;
		}
	}
	printed = false;
	running = false;
}

void initSDL()
{
				if (SDL_Init(SDL_INIT_VIDEO) < 0)
				{
								std::cout<<"Failed to initialise SDL "<<SDL_GetError()<<std::endl;
								exit(1);
				}
				window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, XSIZE * 10, YSIZE * 10, 0);

				if (!window)
				{
								std::cout<<"Failed to open window "<<SDL_GetError()<<std::endl;
								std:exit(1);
				}

				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

				if (!renderer)
				{
								std::cout<<"Failed to create renderer "<<SDL_GetError()<<std::endl;
								std::exit(1);
				}
}

void print_grid()
{
				if (printed == false)
				{
								printed = true;
				}
				else
				{
								for (int i = 0; i < XSIZE + 3; i++)
												std::cout <<"\x1b[A";
				}
				std::cout << std::endl << "+";
				for (int j = 0; j < YSIZE; j++)
								std::cout << "---";
				std::cout << "+" << std::endl;

				for (int i = 0; i < XSIZE; ++i)
				{
								std::cout << "|";
								for (int j = 0; j < YSIZE; ++j)
								{
												if (grid[i][j] == 0)
												{
																std::cout<<"   ";
												}
												else
												{
																std::cout<<" @ ";
												}
								}
								std::cout << "|\n";
				}

				std::cout << "+";
				for (int j = 0; j < YSIZE; j++)
								std::cout << " - ";
				std::cout << "+" << std::endl;
}

void toggle_cell(int i, int j)
{
				grid[i][j] = !grid[i][j];
}

bool grid_at(int i, int j)
{
				if (i >= 0 && j >= 0 && i < XSIZE && j < YSIZE)
								return grid[i][j];
				else
								return 0;
}

int alive_neighbours(int i, int j)
{
				int n = 0;
				for (int l = i - 1; l <= i + 1; ++l)
				{
								for (int k = j - 1; k <= j + 1; ++k)
								{
												n += grid_at(l, k);
								}
				}
				// self is not neighbour
				return n - grid[i][j];
}

void iterate_grid()
{
				int n;
				for (int i = 0; i < XSIZE; ++i)
				{
								for (int j = 0; j < YSIZE; ++j)
								{
												n = alive_neighbours(i, j);
												if (grid[i][j] && (n < 2 || n > 3))
												{
																new_grid[i][j] = 0;
												}
												else if (!grid[i][j] && n == 3)
												{
																new_grid[i][j] = 1;
												}
												else
												{
																new_grid[i][j] = grid[i][j];
												}
								}
				}
				for (int i = 0; i < XSIZE; ++i)
				{
								for (int j = 0; j < YSIZE; ++j)
								{
												grid[i][j] = new_grid[i][j];
								}
				}
}


void draw_grid()
{
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);
				for (int i = 0; i < XSIZE; ++i)
				{
								for (int j = 0; j < YSIZE; ++j)
								{
												if (grid[i][j] == 1)
												{
																SDL_Rect rect = {i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE};
																SDL_SetRenderDrawColor(renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
																SDL_RenderFillRect(renderer, &rect);
												}
												
								}
				}
				SDL_RenderPresent(renderer);
}

void toggle_cell_xy(int x, int y)
{
				toggle_cell(x / CELL_SIZE, y / CELL_SIZE);
}

int main()
{
				init_grid();
				toggle_cell(5, 5);
				toggle_cell(5, 6);
				toggle_cell(5, 7);
				initSDL();
				SDL_Event event;
				bool running = true;
				bool paused = false;
				bool mouse_pressed;
				std::set<std::pair<int, int>> pressed_squares = {};
				std::pair<int, int> square;
				while (running)
				{
								while(SDL_PollEvent (&event) != 0)
								{

												switch (event.type)
												{
																case SDL_QUIT:
																				running = false;
																				break;
																case SDL_MOUSEBUTTONDOWN:
																				mouse_pressed = true;
																				square.first = event.button.x / 10;
																				square.second = event.button.y /10;
																				pressed_squares.insert(square);
																				toggle_cell(square.first, square.second);
																				break;
																case SDL_MOUSEMOTION:
																				if (mouse_pressed == true)
																				{
																								square.first = event.motion.x / 10;
																								square.second = event.motion.y / 10;
																								if (pressed_squares.insert(square).second)
																								{
																												toggle_cell(square.first, square.second);
																								}
																				}
																				break;
																case SDL_MOUSEBUTTONUP:
																				mouse_pressed = false;
																				pressed_squares.clear();
																				break;
																case SDL_KEYUP:
																				paused = !paused;
																default:
																				break;
												}
																
								}
								draw_grid();
								if (!paused)
												iterate_grid();
								SDL_Delay(250);
				}
				SDL_Quit();
				return 0;
}
