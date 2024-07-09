#include <SDL2/SDL.h>
#include "state.h"
#include "colour_functions.h"
#include "fractal_functions.h"

static fractalOut getResult(int x, int y, int width, int height, state* state) {
    double complex z = RE_START + (((float) x / (float) width) * (RE_END - RE_START))
                + (I * (IM_START + (((float) y / (float) height) * (IM_END - IM_START))));
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

static int grayscaleGeneral(int x, int y, int width, int height, SDL_Renderer* r, state* state, bool centreWhite) {
    fractalOut out = getResult(x, y, width, height, state);
    int c;
    if (out.iters == -1) {
        if (centreWhite) {
            c = 255;
        } else {
            c = 0;
        }
    } else {
        int P;
        switch (state->fractalType) {
            case TYPE_MANDELBROT:
                P = 2;
                break;
            case TYPE_JULIA:
                P = state->fractalArgs.julia.P;
                break;
            default:
                fprintf(stderr, "Unknown fractal type %d.\n", state->fractalType);
                exit(EXIT_FAILURE);
                break;
        }
        float logEnd = log(cabs(out.end));
        float newIters = out.iters - (log(logEnd / log(2)) / log(P));
        c = 255 * ((float) newIters / (float) MAX_ITERS);
    }
    if (state->inverted) {
        c = 255 - c;
    }
    int setColor = SDL_SetRenderDrawColor(r, c, c, c, 255);
    int drawPixel;
    if (state->highRes) {
        drawPixel = SDL_RenderDrawPoint(r, x, y);
    } else {
        SDL_Rect rect = {.x = x, .y = y, .w = LOW_RES_SIZE, .h = LOW_RES_SIZE};
        drawPixel = SDL_RenderFillRect(r, &rect);
    }
    if (setColor == 0 && drawPixel == 0) {
        return 0;
    }
    return -1;
}

int grayscaleCentreBlack(int x, int y, int width, int height, SDL_Renderer* r, state* state) {
    return grayscaleGeneral(x, y, width, height, r, state, false);
}

int grayscaleCentreWhite(int x, int y, int width, int height, SDL_Renderer* r, state* state) {
    return grayscaleGeneral(x, y, width, height, r, state, true);
}

