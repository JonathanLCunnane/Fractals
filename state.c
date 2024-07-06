#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "state.h"


state* initState(SDL_Window* w, SDL_Renderer* r) {
    state* state = malloc(sizeof(state));
    assert(state != NULL);

    state->isRunning = true;
    state->window = w;
    state->renderer = r;
    return state;
}

void handleEvents(state* state) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            state->isRunning = false;
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
