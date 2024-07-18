/*
 *  The state holds information about the program's state,
 *  and here are functions to update the state as necessary
 */

#ifndef STATE_H
#define STATE_H

#define TYPE_MANDELBROT 0x0
#define TYPE_JULIA 0x1
#define TYPE_MULTIBROT 0x2
#define TYPE_MULTIJULIA 0x3
#define TYPE_IS_MULTI(type) ((type >> 1) != 0)

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
            double complex c;
        } julia;
        struct {
            double complex c;
            int P;
        } multijulia;
        struct {
            int P;
        } multibrot;
    } fractalArgs;
    double re_start, re_end, im_start, im_end;
    double colourMultiplier; // For changing the number of bands of colour for certain colour modes.
    int max_pixel_size;
    int curr_pixel_size;
    int currX, currY;
    int max_iters;
    int colourOffset; // For cycling the colours in the bands in certain modes.
    int windowWidth, windowHeight;
    uint8_t fractalType; /* See the top of this file for types */
    int (*pixelGetter) (int x, int y, state* state);
    bool isRunning;
    bool redrawRequired;
    bool inverted;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* pixels; // This pointer is to a uint8_t so we can access the individual R, G, and B components of each pixel.
};

extern state* initState(SDL_Window*, SDL_Renderer*, SDL_Texture*, uint32_t*);
extern vec2 getCoord(state*, int, int); // Takes the state and the pixel x and y coordinates on the window and returns the relating coordinate.
extern void handleEvents(state*);
extern void destroyState(state*);

#endif
