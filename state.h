/*
 *  The state holds information about the program's state,
 *  and here are functions to update the state as necessary
 */

#ifndef STATE_H
#define STATE_H

#define TYPE_MANDELBROT 0x0
#define TYPE_JULIA 0x1

#define ZOOM_FACTOR 1.5

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>

typedef struct {
    double x, y;
} vec2;

typedef struct state state;
struct state {
    union {
        struct {
            int P;
            double complex c;
        } julia;
    } fractalArgs;
    double re_start, re_end, im_start, im_end;
    double colourMultiplier; // For changing the number of bands of colour for certain colour modes.
    int colourOffset; // For cycling the colours in the bands in certain modes.
    int windowWidth, windowHeight;
    uint8_t fractalType; /* See the top of this file for types */
    int (*pixelGetter) (int x, int y, state* state);
    bool isRunning;
    bool redrawRequired;
    bool inverted;
    bool highRes;
    SDL_Window* window;
    SDL_Renderer* renderer;
};

extern state* initState();
extern vec2 getCoord(state*, int, int); // Takes the state and the pixel x and y coordinates on the window and returns the relating coordinate.
extern void handleEvents(state*);
extern void destroyState(state*);

#endif
