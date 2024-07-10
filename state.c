#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "colour_functions.h"
#include "state.h"

state* initState(SDL_Window* w, SDL_Renderer* r) {
    state* state = malloc(sizeof(struct state));
    assert(state != NULL);

    state->pixelGetter = &grayscaleCentreBlack;
    state->fractalType = TYPE_MANDELBROT;
    state->colourMultiplier = 1.0;
    state->colourOffset = 0;
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
    pixelGetter prevPixelGetter = state->pixelGetter;
    pixelGetter nextPixelGetter = prevPixelGetter;
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
                /*
                 * CHANGE COLOUR MODE
                 */
                case SDLK_1:
                    nextPixelGetter = &grayscaleCentreBlack;
                    break;
                case SDLK_2:
                    nextPixelGetter = &grayscaleCentreWhite;
                    break;
                case SDLK_3:
                    nextPixelGetter = &rainbowColourCentreBlack;
                    break;
                /*
                 * INVERT COLOURS
                 */
                case SDLK_i:
                    state->redrawRequired = true;
                    state->highRes = false;
                    state->inverted = !(state->inverted);
                    break;
                /*
                 * RESET STATE 
                 */
                case SDLK_r:
                    state->fractalType = TYPE_MANDELBROT;
                    state->colourMultiplier = 1.0;
                    state->colourOffset = 0;
                    state->redrawRequired = true;
                    state->inverted = false;
                    state->highRes = false;
                    break;
                /*
                 * CHANGE COLOUR BAND COUNT VIA MULTIPLIER
                 */
                case SDLK_UP:
                    state->redrawRequired = true;
                    state->highRes = false;
                    state->colourMultiplier *= COLOUR_ADJUST_MULTIPLIER;
                    break;
                case SDLK_DOWN:
                    state->redrawRequired = true;
                    state->highRes = false;
                    state->colourMultiplier /= COLOUR_ADJUST_MULTIPLIER;
                    break;
                /*
                 * CHANGE COLOUR BAND COLOUR VIA OFFSET
                 */
                case SDLK_LEFT:
                    state->redrawRequired = true;
                    state->highRes = false;
                    state->colourOffset++;
                    break;
                case SDLK_RIGHT:
                    state->redrawRequired = true;
                    state->highRes = false;
                    state->colourOffset++;
                    break;
                /*
                 * CONFIRM SELECTION
                 */
                case SDLK_RETURN:
                case SDLK_KP_ENTER:
                    // Confirming selection, use highRes.
                    if (!state->highRes) state->redrawRequired = true;
                    state->highRes = true;
                    break;
                default:
                    break;
            }
            if (prevPixelGetter != nextPixelGetter) {
                // Appearance changes, so use lowRes.
                state->redrawRequired = true;
                state->highRes = false;
                state->pixelGetter = nextPixelGetter;
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
