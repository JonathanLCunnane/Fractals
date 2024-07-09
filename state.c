#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "colour_functions.h"
#include "state.h"


state* initState(SDL_Window* w, SDL_Renderer* r) {
    state* state = malloc(sizeof(struct state));
    assert(state != NULL);

    state->pixelSetter = &grayscaleCentreBlack;
    state->fractalType = TYPE_MANDELBROT;
    state->redrawRequired = true;
    state->inverted = false;
    state->highRes = false;
    state->isRunning = true;
    state->window = w;
    state->renderer = r;
    return state;
}

void handleEvents(state* state) {
    state->redrawRequired = false;
    state->isRunning = true;
    SDL_Event event;
    SDL_PollEvent(&event);
    pixelSetter prevPixelSetter = state->pixelSetter;
    pixelSetter nextPixelSetter = prevPixelSetter;
    switch (event.type) {
        case SDL_QUIT:
            state->isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    state->redrawRequired = true;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    nextPixelSetter = &grayscaleCentreBlack;
                    break;
                case SDLK_2: 
                    nextPixelSetter = &grayscaleCentreWhite;
                    break;
                case SDLK_i:
                    state->redrawRequired = true;
                    state->inverted = !(state->inverted);
                    break;
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    // Confirming selection, use highRes.
                    if (!state->highRes) state->redrawRequired = true;
                    state->highRes = true;
                    break;
                default:
                    break;
            }
            if (prevPixelSetter != nextPixelSetter) {
                // Appearance changes, so use lowRes.
                state->redrawRequired = true;
                state->highRes = false;
                state->pixelSetter = nextPixelSetter;
            }
            break;
        default:
            break;
    }
}

void destroyState(state* state) {
    SDL_DestroyWindow(state->window);
    SDL_DestroyRenderer(state->renderer);
    SDL_Quit();
    free(state);
}
