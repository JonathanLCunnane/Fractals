#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "colour_functions.h"
#include "state.h"

state* initState(SDL_Window* w, SDL_Renderer* r) {
    state* state = malloc(sizeof(struct state));
    assert(state != NULL);
   
    state->re_start = -2.0;
    state->re_end = 2.0;
    state->im_start = -2.0;
    state->im_end = 2.0;
    SDL_GetWindowSize(w, &state->windowWidth, &state->windowHeight);
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

vec2 getCoord(state* state, int x, int y) {
    double xCoord = state->re_start + (((double) x / (double) state->windowWidth) * (state->re_end - state->re_start));
    double yCoord = state->im_end - (((double) y / (double) state->windowHeight) * (state->im_end - state->im_start));
    return (vec2) {
        .x = xCoord, 
        .y = yCoord
    };
}

static void zoomIn(state* state, vec2 at, double factor) {
    double new_im_size = (state->im_end - state->im_start) / factor;
    double new_re_size = (state->re_end - state->re_start) / factor;
    state->re_start = at.x - (new_re_size / 2);
    state->re_end = at.x + (new_re_size / 2);
    state->im_start = at.y - (new_im_size / 2);
    state->im_end = at.y + (new_im_size / 2);
}

void handleEvents(state* state) {
    state->redrawRequired = false;
    state->isRunning = true;
    SDL_Event event;
    SDL_PollEvent(&event);
    pixelGetter prevPixelGetter = state->pixelGetter;
    pixelGetter nextPixelGetter = prevPixelGetter;
    vec2 mouseCoord;
    switch (event.type) {
        case SDL_QUIT:
            state->isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    state->windowWidth = event.window.data1;
                    state->windowHeight = event.window.data2;
                    state->redrawRequired = true;
                    break;
                default:
                    break;
            }
            break;
        /*
         * KEYBOARD HANDLING
         */
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
                    zoomIn(state, mouseCoord, ZOOM_FACTOR);
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
                    state->re_start = -2.0;
                    state->im_end = 2.0;
                    state->im_start = -2.0;
                    state->re_end = 2.0; 
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
                    state->colourOffset--;
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
        /*
         * MOUSE EVENT HANDLING.
         */
        case SDL_MOUSEBUTTONDOWN:
            mouseCoord = getCoord(state, event.button.x, event.button.y);
            state->redrawRequired = true;
            state->highRes = false;
            printf("x, y = %f, %f\n", mouseCoord.x, mouseCoord.y);
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: // Zoom in
                    zoomIn(state, mouseCoord, ZOOM_FACTOR);
                    break;
                case SDL_BUTTON_MIDDLE: // Switch to Julia at point
                    state->fractalType = TYPE_JULIA;
                    state->fractalArgs.julia.P = 2;
                    state->fractalArgs.julia.c = mouseCoord.x + (mouseCoord.y * I);
                    break;
                case SDL_BUTTON_RIGHT: // Zoom out
                    zoomIn(state, mouseCoord, 1 / ZOOM_FACTOR);
                    break;
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
