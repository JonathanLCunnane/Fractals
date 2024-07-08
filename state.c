#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>

#include "state.h"


state* initState(SDL_Window* w, SDL_Renderer* r) {
    state* state = malloc(sizeof(state));
    assert(state != NULL);

    state->redrawRequired = true;
    state->isRunning = true;
    state->window = w;
    state->renderer = r;
    return state;
}

void handleEvents(state* state) {
    state->redrawRequired = false;
    state->isRunning = true;
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            state->isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    state->redrawRequired = true;
                    break;
                default:
                    break;
            }
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
