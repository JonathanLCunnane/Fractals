#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "state.h"
#include "fractal_functions.h"
#include "colour_functions.h"

#define SDL_ASSERT(x) assert(x == 0);

static int drawCurrRectangle(state* state);

void initialiseSDL(SDL_Window** window, SDL_Renderer** renderer, SDL_Texture** texture, state** state) {
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

    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, 1000, 1000);

    uint32_t* pixels = malloc(1000 * 1000 * sizeof(uint32_t));

    *state = initState(*window, *renderer, *texture, pixels);
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
    SDL_Window* window; SDL_Renderer* renderer; SDL_Texture* texture; state* state;
    initialiseSDL(&window, &renderer, &texture, &state);
    state->pixelGetter = setter;
    
    while (state->isRunning) {
        // If required update the screen.
        if (state->redrawRequired) {
            state->curr_pixel_size = state->max_pixel_size;
            
            SDL_ASSERT(SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255));
            SDL_ASSERT(SDL_RenderClear(renderer));
            
            // Clear the screen to the draw colour.
            SDL_RenderPresent(renderer);
        }
        
        SDL_ASSERT(drawCurrRectangle(state));

        handleEvents(state);
    }

    return EXIT_SUCCESS;
}

int drawCurrRectangle(state* state) {
    if (state->curr_pixel_size == 0) {
        return 0;
    }
    if (state->currX > state->windowWidth) {
        state->currX = 0;
        state->currY += state->curr_pixel_size;
        if (state->currY > state->windowWidth) {
            state->currY = 0;
            state->curr_pixel_size /= 3;

            // We draw what we have so far.
            SDL_UpdateTexture(state->texture, NULL, state->pixels, sizeof(uint32_t) * state->windowWidth);
            SDL_RenderClear(state->renderer);
            SDL_RenderCopy(state->renderer, state->texture, NULL, NULL);
            SDL_RenderPresent(state->renderer);
        }
    }
    int colour = state->pixelGetter(state->currX, state->currY, state);
    int success = setPixel(state->currX, state->currY, colour, state);
    state->currX += state->curr_pixel_size;
    return success;
}

