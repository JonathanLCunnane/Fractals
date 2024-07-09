#ifndef COLOUR_FUNCTIONS_H
#define COLOUR_FUNCTIONS_H

#include <SDL2/SDL.h>
#include "state.h"

#define RE_START -2.0f
#define RE_END 2.0f
#define IM_START -2.0f
#define IM_END 2.0f

#define LOW_RES_SIZE 5

// A pixelSetter takes in x and y values, the width and height of the window,
// and sets the relevant pixel based on the iterations. It returns 0 on success.
// The x and y coordinates are counted from the top left.
typedef int (*pixelSetter) (int x, int y, int width, int height, SDL_Renderer* r, state* state);
extern int grayscaleCentreBlack(int x, int y, int width, int height, SDL_Renderer* r, state* state);
extern int grayscaleCentreWhite(int x, int y, int width, int height, SDL_Renderer* r, state* state);

#endif
