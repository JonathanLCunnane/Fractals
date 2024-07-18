#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "colour_functions.h"
#include "state.h"

#define MAX_WINDOW_TITLE_LENGTH 200

state* initState(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t, uint32_t* pixels) {
    state* state = malloc(sizeof(struct state));
    assert(state != NULL);
   
    state->re_start = -2.0;
    state->re_end = 2.0;
    state->im_start = -2.0;
    state->im_end = 2.0;
    state->max_iters = 20;
    state->max_pixel_size = 100;
    state->curr_pixel_size = state->max_pixel_size;
    state->currX = 0; state->currY = 0;
    SDL_GetWindowSize(w, &state->windowWidth, &state->windowHeight);
    state->pixelGetter = &grayscaleCentreBlack;
    state->fractalType = TYPE_MANDELBROT;
    state->colourMultiplier = 1.0;
    state->colourOffset = 0;
    state->redrawRequired = true;
    state->inverted = false;
    state->isRunning = true;
    state->window = w;
    state->renderer = r;
    state->texture = t;
    state->pixels = pixels;
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

static void updateWindowText(state* state, vec2 mouseCoords) {
    char buf[MAX_WINDOW_TITLE_LENGTH];
    char* mode;
    switch (state->fractalType) {
        case TYPE_MANDELBROT:
            mode = "Mandelbrot";
            break;
        case TYPE_JULIA:
            mode = "Julia";
            break;
    }
    sprintf(
        buf, "Fractals:: %d iterations : %s : %.5f + %.5fi",
        state->max_iters, mode, mouseCoords.x, mouseCoords.y
    );
    SDL_SetWindowTitle(state->window, buf);
}

void handleEvents(state* state) {
    state->redrawRequired = false;
    state->isRunning = true;
    SDL_Event event;
    SDL_PollEvent(&event);
    pixelGetter prevPixelGetter = state->pixelGetter;
    pixelGetter nextPixelGetter = prevPixelGetter;
    vec2 mouseCoords;
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
                    // New texture and pixels required.
                    state->texture = SDL_CreateTexture(
                        state->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC,
                        state->windowWidth, state->windowHeight
                    );
                    free(state->pixels);
                    state->pixels = malloc(sizeof(uint32_t) * state->windowWidth * state->windowHeight); // The 3 is for the 3 components R, G, and B.
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
                case SDLK_4:
                    nextPixelGetter = &blueBrownWhiteCentreBlack;
                    break;
                /*
                 * INVERT COLOURS
                 */
                case SDLK_i:
                    state->redrawRequired = true;
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
                    break;
                /*
                 * CHANGE COLOUR BAND COUNT VIA MULTIPLIER
                 */
                case SDLK_UP:
                    state->redrawRequired = true;
                    state->colourMultiplier *= COLOUR_ADJUST_MULTIPLIER;
                    break;
                case SDLK_DOWN:
                    state->redrawRequired = true;
                    state->colourMultiplier /= COLOUR_ADJUST_MULTIPLIER;
                    break;
                /*
                 * CHANGE COLOUR BAND COLOUR VIA OFFSET
                 */
                case SDLK_LEFT:
                    state->redrawRequired = true;
                    state->colourOffset--;
                    break;
                case SDLK_RIGHT:
                    state->redrawRequired = true;
                    state->colourOffset++;
                    break;
                default:
                    break;
            }
            if (prevPixelGetter != nextPixelGetter) {
                // Appearance changes, so use lowRes.
                state->redrawRequired = true;
                state->pixelGetter = nextPixelGetter;
            }
            break;
        /*
         * MOUSE EVENT HANDLING.
         */
        case SDL_MOUSEBUTTONDOWN:
            mouseCoords = getCoord(state, event.button.x, event.button.y);
            state->redrawRequired = true;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: // Zoom in
                    zoomIn(state, mouseCoords, ZOOM_FACTOR);
                    break;
                case SDL_BUTTON_MIDDLE: // Switch to Julia at point
                    state->fractalType = TYPE_JULIA;
                    state->fractalArgs.julia.P = 2;
                    state->fractalArgs.julia.c = mouseCoords.x + (mouseCoords.y * I);
                    updateWindowText(state, mouseCoords);
                    break;
                case SDL_BUTTON_RIGHT: // Zoom out
                    zoomIn(state, mouseCoords, 1 / ZOOM_FACTOR);
                    break;
            }
            break;
        case SDL_MOUSEMOTION: // Adjust window title.
            mouseCoords = getCoord(state, event.motion.x, event.motion.y);
            updateWindowText(state, mouseCoords);
            break;
        case SDL_MOUSEWHEEL: // Adjust iterations.
            state->max_iters += 10 * event.wheel.y;
            if (state->max_iters < 20) state->max_iters = 20;
            updateWindowText(state, mouseCoords);
            state->redrawRequired = true;
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
