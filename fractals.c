#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "state.h"
#include "fractal_functions.h"
#include "colour_functions.h"

#define SDL_ASSERT(x) assert(x == 0);

// For benchmarking
#define BMRK

// drawPixels iterates through all the pixels on the screen and draws them.
static int drawPixels(SDL_Renderer* r, state* state);

void initialiseSDL(SDL_Window** window, SDL_Renderer** renderer, state** state) {
    // Initialises the SDL library.
    SDL_ASSERT(SDL_Init(SDL_INIT_EVERYTHING));
    
    // Creating the window with title "Fractals", centered in the screen, size 600x400, shown.
    *window = SDL_CreateWindow(
        "Fractals", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 1000,
        SDL_WINDOW_SHOWN
    );
    assert(*window != NULL);
    
    // Using the first rendering driver available to initialise with no flags
    *renderer = SDL_CreateRenderer(*window, -1, 0); 
    assert(*renderer != NULL);

    *state = initState(*window, *renderer);
    assert(*state != NULL);
}

int main(void) {
    /*
     * PARSE COMMAND LINE ARGUMENTS
     */

    pixelGetter setter = &grayscaleCentreBlack;

    /*
     *  INITIALISE AND RUN SDL
     */
    SDL_Window* window; SDL_Renderer* renderer; state* state;
    initialiseSDL(&window, &renderer, &state);
    state->pixelGetter = setter;
    
    while (state->isRunning) {
        // If required update the screen.
        if (state->redrawRequired) {
            // Clear the screen to the draw colour.
            SDL_ASSERT(SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255));
            SDL_ASSERT(SDL_RenderClear(renderer));

            // Draw fractal pixels.
#ifdef BMRK
            clock_t begin = clock();
#endif
            SDL_ASSERT(drawPixels(renderer, state));
#ifdef BMRK
            clock_t end = clock();
            double time_spent = (double)(end - begin)/ CLOCKS_PER_SEC;
            printf("Time spent to draw pixels: %lfs\n", time_spent);
#endif

            // Update the screen.
            SDL_RenderPresent(renderer);
        }

        handleEvents(state);
    }

    return EXIT_SUCCESS;
}

int drawPixels(SDL_Renderer* r, state* state) {
    int inc = state->highRes ? 1 : LOW_RES_SIZE;
    for (int x = 0; x < state->windowWidth; x += inc) {
        for (int y = 0; y < state->windowHeight; y += inc) {
            // Set pixel.
            int colour = state->pixelGetter(x, y, state);
            int success = setPixel(x, y, colour, r, state);
            if (success != 0) return -1;
        }
    }
    return 0;
}
