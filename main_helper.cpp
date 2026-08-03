
#include "headers/main_helper.hpp"
#include "headers/opengles.hpp"
// #include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

void handleResize(AppState *state, SDL_Event *event) {

    int w,h;
    SDL_GetWindowSizeInPixels(state->window, &w, &h);

    glViewport(0, 0, w, h);
}

// 
void handleWASD(AppState *state) {

    int arrSize;
    const bool* keyStates = SDL_GetKeyboardState(&arrSize);

    if (keyStates[SDL_SCANCODE_W]) {
      state->camera->cameraPos += 
      state->camera->cameraFront * state->camera->cameraSpeed;
    }

    if (keyStates[SDL_SCANCODE_S]) {
      state->camera->cameraPos -=
          state->camera->cameraFront * state->camera->cameraSpeed;
    }

    if (keyStates[SDL_SCANCODE_A]) {
      state->camera->cameraPos -=
          glm::normalize(glm::cross(state->camera->cameraFront, state->camera->cameraUp)) * state->camera->cameraSpeed;
    }

    if (keyStates[SDL_SCANCODE_D]) {
      state->camera->cameraPos +=
          glm::normalize(
              glm::cross(state->camera->cameraFront, state->camera->cameraUp)) *
          state->camera->cameraSpeed;
    }
}

void handleScroll(AppState *state, SDL_Event *event) {

    float y = event->wheel.y;

    state->fov -= y;

    if (state->fov < 1.0f) {
        state->fov = 1.0f;
    } 

    if (state->fov > 45.0f) {
        state->fov = 45.0f;
    }
}