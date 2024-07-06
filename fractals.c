#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "state.h"

#define SDL_ASSERT(x) assert(x == 0);

int main(void) {
    // Initialises the SDL library.
    SDL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING));
    
    // Creating the window with title "Fractals", centered in the screen, size 600x400, shown.
    SDL_Window* window = SDL_CreateWindow(
        "Fractals", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        600, 400,
        SDL_WINDOW_SHOWN
    );
    assert(window != NULL);
    
    // Using the first rendering driver available to initialise with no flags
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); 
    assert(renderer != NULL);

    state* state = initState(window, renderer);
    assert(state != NULL);
    
    while (state->isRunning) {
        handleEvents(state);

        // Clear the screen to the draw colour.
        SDL_ASSERT(SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255));
        SDL_ASSERT(SDL_RenderClear(renderer));

        // Draw fractal pixels.

        // Update the screen.
        SDL_RenderPresent(renderer);
    }


    return EXIT_SUCCESS;
}
