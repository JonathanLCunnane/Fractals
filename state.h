/*
 *  The state holds information about the program's state,
 *  and here are functions to update the state as necessary
 */

#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct {
    bool isRunning;
    bool redrawRequired;
    SDL_Window* window;
    SDL_Renderer* renderer;
} state;

state* initState();
void handleEvents(state*);
void destroyState(state*);

#endif
