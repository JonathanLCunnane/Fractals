#ifndef COLOUR_FUNCTIONS_H
#define COLOUR_FUNCTIONS_H

#include <SDL2/SDL.h>
#include "state.h"

#define RED_COMPONENT(x) ((x & (0xFF << 16)) >> 16)
#define GREEN_COMPONENT(x) ((x & (0xFF << 8)) >> 8)
#define BLUE_COMPONENT(x) (x & 0xFF)
#define COLOUR(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define INVERTED_COLOUR(x) COLOUR(255 - RED_COMPONENT(x), 255 - GREEN_COMPONENT(x), 255 - BLUE_COMPONENT(x))

#define COLOUR_ADJUST_MULTIPLIER 1.5

// Interpolate from colour 'start' to 'end'. t is in between 0 and 1. 
// If start = 0xFF00AA, then R = 0xFF, G = 0x00, B = 0xAA
extern int linearInterpolate(int start, int end, double t);

// setPixel takes the colour which we need to set the pixel
// and the information in state to set the pixel correctly.
// Returns -1 on failure, 0 on success.
extern int setPixel(int x, int y, int colour, state* state);

// A pixelGetter takes in x and y values, the width and height of the window,
// and gets the relevant pixel colour based on the iterations. It returns the colour to set the pixel.
// The x and y coordinates are counted from the top left.
typedef int (*pixelGetter) (int x, int y, state* state);
extern int grayscaleCentreBlack(int x, int y, state* state);
extern int grayscaleCentreWhite(int x, int y, state* state);
extern int rainbowColourCentreBlack(int x, int y, state* state);
extern int blueBrownWhiteCentreBlack(int x, int y, state* state);

#endif
