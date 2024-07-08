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

typedef struct {
    uint8_t fractalType;
    union {
        struct {
            int P;
            double complex c;
        } julia;
    } fractalArgs;
    bool isRunning;
    bool redrawRequired;
    SDL_Window* window;
    SDL_Renderer* renderer;
} state;
// fractalType = 0x0 -> Mandelbrot (DEFAULT)
// fractalType = 0x1 -> Julia

state* initState();
void handleEvents(state*);
void destroyState(state*);

#endif
