#include <SDL2/SDL.h>
#include "state.h"
#include "colour_functions.h"
#include "fractal_functions.h"

static int rainbowColours[] = {
    0xe81416, // Red
    0xffa500, // Orange
    0xfaeb36, // Yellow
    0x79c314, // Green
    0x487de7, // Blue
    0x4b369d, // Indigo
    0x70369d, // Violet
};

int linearInterpolate(int start, int end, double t) {
    return COLOUR(
        (int) (RED_COMPONENT(start) + (t * (RED_COMPONENT(end) - RED_COMPONENT(start)))),
        (int) (GREEN_COMPONENT(start) + (t * (GREEN_COMPONENT(end) - GREEN_COMPONENT(start)))),
        (int) (BLUE_COMPONENT(start) + (t * (BLUE_COMPONENT(end) - BLUE_COMPONENT(start))))
    );
}

int setPixel(int x, int y, int colour, SDL_Renderer* r, state* state) {
    int setColour;
    if (state->inverted) {
        setColour = SDL_SetRenderDrawColor(
            r, 
            255 - RED_COMPONENT(colour), 
            255 - GREEN_COMPONENT(colour), 
            255 - BLUE_COMPONENT(colour), 
            255
        );
    } else {
        setColour = SDL_SetRenderDrawColor(
            r, 
            RED_COMPONENT(colour), 
            GREEN_COMPONENT(colour), 
            BLUE_COMPONENT(colour), 
            255
        );
    }
    int drawPixel;
    if (state->highRes) {
        drawPixel = SDL_RenderDrawPoint(r, x, y);
    } else {
        SDL_Rect rect = {.x = x, .y = y, .w = LOW_RES_SIZE, .h = LOW_RES_SIZE};
        drawPixel = SDL_RenderFillRect(r, &rect);
    }
    if (setColour == 0 && drawPixel == 0) {
        return 0;
    }
    return -1;
}

static fractalOut getResult(int x, int y, state* state) {
    vec2 coord = getCoord(state, x, y);
    double complex z = coord.x + (I * coord.y);
    switch (state->fractalType) {
        case TYPE_MANDELBROT:
            return mandelbrot(z);
            break;
        case TYPE_JULIA:
            return julia(
                state->fractalArgs.julia.P,
                z,
                state->fractalArgs.julia.c
            );
            break;
        default:
            fprintf(stderr, "Unknown fractal type %d.\n", state->fractalType);
            exit(EXIT_FAILURE);
            break;
    }
}

static int grayscaleGeneral(int x, int y, state* state, bool centreWhite) {
    fractalOut out = getResult(x, y, state);
    int c;
    if (out.iters == -1) {
        if (centreWhite) {
            c = 255;
        } else {
            c = 0;
        }
               
    } else {
        c = 255 * ((double) out.smoothIters / (double) MAX_ITERS);
    }
    return COLOUR(c, c, c);
}

static int getOutsideLerp(fractalOut out, state* state, int numColours, int* colours) {
    int smoothItersFloor = floor(out.smoothIters * state->colourMultiplier);
    int colourIDX = smoothItersFloor + state->colourOffset;
    int startColour = colours[colourIDX % numColours];
    int endColour = colours[(colourIDX + 1) % numColours];
    return linearInterpolate(startColour, endColour, (out.smoothIters * state->colourMultiplier) - smoothItersFloor);
}

static int outsideColourGeneral(
    int x, int y, state* state, 
    bool centreWhite, int numColours, int* colours
) {
    fractalOut out = getResult(x, y, state);
    int c;
    if (out.iters == -1) {
        if (centreWhite) {
            c = 0xFFFFFF;
        } else {
            c = 0x000000;
        }
    } else {
        c = getOutsideLerp(out, state, numColours, colours);
    }
    return c;
}

int grayscaleCentreBlack(int x, int y, state* state) {
    return grayscaleGeneral(x, y, state, false);
}

int grayscaleCentreWhite(int x, int y, state* state) {
    return grayscaleGeneral(x, y, state, true);
}

int rainbowColourCentreBlack(int x, int y, state* state) {
    return outsideColourGeneral(x, y, state, false, 7, rainbowColours);
}
