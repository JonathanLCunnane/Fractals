#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <SDL2/SDL.h>

#include "state.h"

#define SDL_ASSERT(x) assert(x == 0);
#define MAX_ITERS 20
#define RE_START -2.0f
#define RE_END 2.0f
#define IM_START -2.0f
#define IM_END 2.0f

// A pixelFunc takes an x and y values, and sets the
// relevant pixel. It returns 0 on success.
// The x and y coordinates are counted from the top left.
typedef int (*pixelSetter) (int x, int y, int width, int height, SDL_Renderer* r);
static int mandlebrot(int x, int y, int width, int height, SDL_Renderer* r);

// drawPixels iterates through all the pixels on the screen and draws them.
static int drawPixels(SDL_Renderer* r, SDL_Window* w, pixelSetter ps);

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

    pixelSetter setter = &mandlebrot;

    /*
     *  INITIALISE AND RUN SDL
     */
    SDL_Window* window; SDL_Renderer* renderer; state* state;
    initialiseSDL(&window, &renderer, &state);
    
    while (state->isRunning) {
        handleEvents(state);

        // Clear the screen to the draw colour.
        SDL_ASSERT(SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255));
        SDL_ASSERT(SDL_RenderClear(renderer));

        // Draw fractal pixels.
        SDL_ASSERT(drawPixels(renderer, window, setter));

        // Update the screen.
        SDL_RenderPresent(renderer);
    }

    return EXIT_SUCCESS;
}

static int mandlebrotIters(double x, double y) {
    double complex c = x + (y * I);
    double complex z = 0;
    int n = 0;
    while (cabs(z) < 2 && n < MAX_ITERS) {
        z = cpow(z, 2) + c;
        n++;
    }
    return n;
}

int mandlebrot(int x, int y, int width, int height, SDL_Renderer* r) {
    double xmandle = RE_START + (((float) x / (float) width) * (RE_END - RE_START));
    double ymandle = IM_START + (((float) y / (float) height) * (IM_END - IM_START));
    int iters = mandlebrotIters(xmandle, ymandle);
    int c = 255 - (255 * ((float) iters / (float) MAX_ITERS));
    if (
        SDL_SetRenderDrawColor(r, c, c, c, 255) == 0 &&
        SDL_RenderDrawPoint(r, x, y) == 0
    ) {
        return 0;
    }
    return -1;
}

int drawPixels(SDL_Renderer* r, SDL_Window* w, pixelSetter ps) {
    int width;
    int height;
    SDL_GetWindowSize(w, &width, &height);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (ps(x, y, width, height, r) != 0) return -1;
        }
    }
    return 0;
}
