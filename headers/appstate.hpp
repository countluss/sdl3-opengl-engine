
#pragma once

#include "headers/camera.hpp"
#include "headers/mesh.hpp"
#include "headers/shader.hpp"
#include "headers/texture.hpp"
#include "headers/transform.hpp"

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <vector>
#include "headers/entity.hpp"

struct AppState {
    SDL_Window *window;
    SDL_GLContext gles_context;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Mesh> mesh;
    
    std::unique_ptr<Camera> camera;

    Uint64 lastFrame = 0;
    Uint64 deltaTime = 0;
    float deltaSeconds = 0.0f;

    float lastMouseX = 400, lastMouseY = 300;
    bool firstMouse = true;
    float mouseSensitivity = 0.1f;

    float fov = 45.0f;

    glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, 1.0f);
    std::unique_ptr<Texture> specularMap;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<glm::vec3> cubePositions;
};