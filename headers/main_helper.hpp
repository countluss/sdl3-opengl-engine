
#pragma once

#include "appstate.hpp"
#include <SDL3/SDL_events.h>

void handleResize(AppState *state, SDL_Event *event);

void handleInput(AppState *state, SDL_Event *event);

void handleWASD(AppState *state);

void handleScroll(AppState *state, SDL_Event *event);