#include <iostream>
#include "cgol-grid.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "cgol-grid-display.h"
#include "sdl-event-observable.h"
#include "cgol-cell-creator.h"
#include "cgol-time-control.h"

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	std::unique_ptr<SDL_Window*> pWindow;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &window, &renderer);
	TTF_Font* OswaldRegular = TTF_OpenFont("Oswald-Regular.ttf", 24);
	if (OswaldRegular == NULL) {
		std::cerr << TTF_GetError();
	}
	cgol::Grid grid;
	cgol::GridDisplay gridDisplay(grid);
	sdl::EventObservable eventObservable;
	cgol::CellCreator cellCreator(grid, gridDisplay.getCamera());
	cgol::TimeControl timeControl(renderer, OswaldRegular);

	eventObservable.subscribe(gridDisplay);
	eventObservable.subscribe(cellCreator);
	eventObservable.subscribe(timeControl);
	/*grid.setAlive({ 10, 10 });
	grid.setAlive({ 10, 11 });
	grid.setAlive({ 10, 12 });
	grid.setAlive({ 9, 12 });
	grid.setAlive({ 8, 11 });*/

	for (int i = 0; i < 20; ++i)
	{
		grid.setAlive({ i, 40 });
	}

	while (true)
	{
		// handle events
		eventObservable.pollEvents();

		// update app
		timeControl.update(grid);

		// render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		gridDisplay.render(renderer);
		timeControl.render(renderer);
		SDL_RenderPresent(renderer);

		SDL_Delay(30);
	}

	TTF_CloseFont(OswaldRegular);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}