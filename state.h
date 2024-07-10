/*
 *  The state holds information about the program's state,
 *  and here are functions to update the state as necessary
 */

#ifndef STATE_H
#define STATE_H

#define TYPE_MANDELBROT 0x0
#define TYPE_JULIA 0x1

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>

typedef struct state state;
struct state {
    union {
        struct {
            int P;
            double complex c;
        } julia;
    } fractalArgs;
    uint8_t fractalType; /* See the top of this file for types */
    int (*pixelGetter) (int x, int y, int width, int height, state* state);
    bool isRunning;
    bool redrawRequired;
    bool inverted;
    bool highRes;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

state* initState();
void handleEvents(state*);
void destroyState(state*);

#endif
