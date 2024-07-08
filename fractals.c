#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <SDL2/SDL.h>

#include "state.h"
#include "fractal_functions.h"

#define SDL_ASSERT(x) assert(x == 0);
#define RE_START -2.0f
#define RE_END 2.0f
#define IM_START -2.0f
#define IM_END 2.0f


// A pixelFunc takes in x and y values, the width and height of the window,
// and sets the relevant pixel based on the iterations. It returns 0 on success.
// The x and y coordinates are counted from the top left.
typedef int (*pixelSetter) (int x, int y, int width, int height, SDL_Renderer* r, state* state);
static int grayscale(int x, int y, int width, int height, SDL_Renderer* r, state* state);

// drawPixels iterates through all the pixels on the screen and draws them.
static int drawPixels(SDL_Renderer* r, SDL_Window* w, pixelSetter ps, state* state);

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

    pixelSetter setter = &grayscale;

    /*
     *  INITIALISE AND RUN SDL
     */
    SDL_Window* window; SDL_Renderer* renderer; state* state;
    initialiseSDL(&window, &renderer, &state);
    
    while (state->isRunning) {
        // If required update the screen.
        if (state->redrawRequired) {
            // Clear the screen to the draw colour.
            SDL_ASSERT(SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255));
            SDL_ASSERT(SDL_RenderClear(renderer));

            // Draw fractal pixels.
            SDL_ASSERT(drawPixels(renderer, window, setter, state));

            // Update the screen.
            SDL_RenderPresent(renderer);
        }

        handleEvents(state);
    }

    return EXIT_SUCCESS;
}

// Takes pixel coordinates x and y and a state,
// returns the iterations for divergence of the complex
// number represented by that pixel.
static int getIters(int x, int y, int width, int height, state* state) {
    double complex z = RE_START + (((float) x / (float) width) * (RE_END - RE_START))
                + (I * (IM_START + (((float) y / (float) height) * (IM_END - IM_START))));
    switch (state->fractalType) {
        case TYPE_MANDELBROT:
            return mandelbrotIters(z);
            break;
        case TYPE_JULIA:
            return juliaIters(
                state->fractalArgs.julia.P,
                z,
                state->fractalArgs.julia.c
            );
            break;
        default:
            fprintf(stderr, "Unknown fractal type %d.\n", state->fractalType);
            return -1;
            break;
    }
}

int grayscale(int x, int y, int width, int height, SDL_Renderer* r, state* state) {
    int iters = getIters(x, y, width, height, state);
    int c;
    if (iters == -1) {
        c = 0;
    } else {
        c = 255 * ((float) iters / (float) MAX_ITERS);
    }
    if (
        SDL_SetRenderDrawColor(r, c, c, c, 255) == 0 &&
        SDL_RenderDrawPoint(r, x, y) == 0
    ) {
        return 0;
    }
    return -1;
}

int drawPixels(SDL_Renderer* r, SDL_Window* w, pixelSetter ps, state* state) {
    int width;
    int height;
    SDL_GetWindowSize(w, &width, &height);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (ps(x, y, width, height, r, state) != 0) return -1;
        }
    }
    return 0;
}
