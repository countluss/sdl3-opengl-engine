
// Include SDL related libraries

#include "external/glm/ext/matrix_clip_space.hpp"
#include "external/glm/ext/matrix_float4x4.hpp"
#include "external/glm/ext/vector_float3.hpp"
#include "external/glm/geometric.hpp"
#include "external/glm/gtc/type_ptr.hpp"
#include <GLES3/gl31.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <memory>
#include <vector>
// This is needed for using the SDL main functions (AppInit, AppIterate, AppEvent, AppQuit)
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

// Include custom libraries
#include "headers/appstate.hpp"
#include "headers/entity.hpp"
#include "headers/main_helper.hpp"
#include "headers/mesh.hpp"
#include "headers/opengles.hpp"
#include "headers/shader.hpp"
#include "headers/texture.hpp"
#include "headers/transform.hpp"

#include "external/glm/mat4x4.hpp"
#include "external/glm/ext/matrix_transform.hpp"
#include "external/glm/vec3.hpp"
#include "external/glm/trigonometric.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    AppState *state = new AppState();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    // Set OpenGL attributes, using 3.1 ES for better android support
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    state->window = SDL_CreateWindow("SDL App", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    state->gles_context = SDL_GL_CreateContext(state->window);

    if (!state->window || !state->gles_context) {
        return SDL_APP_FAILURE;
    }

    int w,h;
    SDL_GetWindowSizeInPixels(state->window, &w, &h);
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);

    // Try to enable Adaptive Vsync or Vsync or Immediate
    if (!SDL_GL_SetSwapInterval(-1)) {
        if (!SDL_GL_SetSwapInterval(1)) {
            if (!SDL_GL_SetSwapInterval(0)) {
                return SDL_APP_FAILURE;
            }
        }
    }

    // Position, Color, Texture Coords
    // Each face needs its own 4 vertices for correct UV mapping (24 total)
    std::vector<Vertex> vertices = {

        // Front face (z = 0.0)
        {{-0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},

        // Back face (z = -1.0)
        {{ 0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},
        {{-0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},

        // Top face (y = 0.5)
        {{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},

        // Bottom face (y = -0.5)
        {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}},

        // Left face (x = -0.5)
        {{-0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},

        // Right face (x = 0.5)
        {{ 0.5f, -0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},

    };

    std::vector<unsigned int> indices = {

        // Front face
        0,
        1,
        2,
        0,
        2,
        3,

        // Back face
        4,
        5,
        6,
        4,
        6,
        7,
  
        // Top face
        8,     
        9,
        10,      
        11, 
        8,
        10,
 
        // Bottom face
        12,
        13,
        14,
        12,
        14,
        15,

        // Left face
        16,
        17,
        18,
        16,
        18,
        19,

        // Right face
        20,
        21,
        22,
        20,
        22,
        23,

    };

    state->cubePositions = std::vector<glm::vec3> {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    Mesh mesh(0, 1, 2, vertices, indices);
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Texture texture("assets/textures/Dirt_2.png", 1);

    auto entity = std::make_unique<Entity>(std::move(mesh), std::move(shader),
                                           std::move(texture));
    state->entities.push_back(std::move(entity));

    Mesh mesh2(0, 1, 2, vertices, indices);
    Shader shader2("shaders/vertex.glsl", "shaders/lightObject.glsl");
    Texture texture2("assets/textures/container.png", 2);

    state->specularMap = std::make_unique<Texture>("assets/textures/specularMap.png", 3);

    auto lightObject = std::make_unique<Entity>(
        std::move(mesh2),
        std::move(shader2),
        std::move(texture2));
    state->entities.push_back(std::move(lightObject));

    Mesh mesh3(0, 1, 2, vertices, indices);
    Shader shader3("shaders/vertex.glsl", "shaders/lightSource.glsl");

    auto lightSource = std::make_unique<Entity>(
        std::move(mesh3),
        std::move(shader3),
        std::nullopt);
    state->entities.push_back(std::move(lightSource));

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    SDL_SetWindowRelativeMouseMode(state->window, true);
    state->camera = std::make_unique<Camera>(cameraPos, cameraFront, cameraUp, 0.05f);

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {

    AppState *state = (AppState *)appstate;

    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        handleResize(state, event);
    } else if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {
        handleScroll(state, event);
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

    delete (AppState *)appstate;
    SDL_Quit();
}

SDL_AppResult SDL_AppIterate(void *appstate) {

  AppState *state = (AppState *)appstate;

  handleWASD(state);

//   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int w,h;
  SDL_GetWindowSizeInPixels(state->window, &w, &h);

  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 projection = glm::perspective(glm::radians(state->fov), (float)w / (float)h, 0.1f, 100.0f);

  // lookAt function creates the view matrix
  glm::mat4 view;

  float mouseDX, mouseDY;
  SDL_GetRelativeMouseState(&mouseDX, &mouseDY);
  mouseDY *= -1;

  state->camera->pitch += mouseDY * state->mouseSensitivity;
  state->camera->yaw += mouseDX * state->mouseSensitivity;

  if (state->camera->pitch > 89.0f)
    state->camera->pitch = 89.0f;
  if (state->camera->pitch < -89.0f)
    state->camera->pitch = -89.0f;

  view = state->camera->getViewMatrix();

  auto& entity1 = state->entities[0];
  auto& lightObject = state->entities[1];
  auto& lightSource = state->entities[2];

  Transform transform1(model, view, projection);
  entity1->updateTransform(transform1);
//   entity1->useEntity();

  Transform transform2(glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)), view, projection);
  lightObject->updateTransform(transform2);
  lightObject->useEntity();

  int objectColor = glGetUniformLocation(lightObject->shader.getProgram(), "uObjectColor");
  glUniform3f(objectColor, 1.0f, 0.5f, 0.31f);
  int lightColor = glGetUniformLocation(lightObject->shader.getProgram(), "uLightColor");
  glUniform3f(lightColor, 1.0f, 1.0f, 1.0f);
  int lightPosLocation = glGetUniformLocation(lightObject->shader.getProgram(), "uLightPos");
  glm::vec3 lightPos = state->lightPos;
  glUniform3f(lightPosLocation , lightPos.x, lightPos.y, lightPos.z);
  int normalLocation = glGetUniformLocation(lightObject->shader.getProgram(), "uNormal");
  glm::mat3 normal = glm::transpose(glm::inverse(model));
  glUniformMatrix3fv(normalLocation, 1, GL_FALSE, glm::value_ptr(normal));
  int viewLocation = glGetUniformLocation(lightObject->shader.getProgram(), "uViewPos");
  glUniform3f(viewLocation, state->camera->cameraPos.x, state->camera->cameraPos.y, state->camera->cameraPos.z);

  int ambientLocation = glGetUniformLocation(lightObject->shader.getProgram(), "material.ambient");
  glUniform3f(ambientLocation, 1.0f, 0.5f, 0.31f);
  int diffuseLocation = glGetUniformLocation(lightObject->shader.getProgram(), "material.diffuse");
  glUniform3f(diffuseLocation, 1.0f, 0.5f, 0.31f);
  int specularLocation = glGetUniformLocation(lightObject->shader.getProgram(), "material.specular");
  glUniform3f(specularLocation, 0.5f, 0.5f, 0.5f);
  int shininessLocation = glGetUniformLocation(lightObject->shader.getProgram(), "material.shininess");
  glUniform1f(shininessLocation, 32.0f);

  glm::vec3 colorLight = glm::vec3(1.0f);

  glm::vec3 diffuseColor = colorLight * glm::vec3(0.2f);
  glm::vec3 ambientColor = colorLight * glm::vec3(0.2f);

  lightObject->shader.setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
  lightObject->shader.setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
  lightObject->shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//   lightObject->shader.setVec3("light.direction",  -0.2f, -1.0f, -0.3f);
  lightObject->shader.setFloat("light.constant", 1.0f);
  lightObject->shader.setFloat("light.linear", 0.09f);
  lightObject->shader.setFloat("light.quadratic", 0.032f);
  lightObject->shader.setInt("material.diffuse", lightObject->texture->getUnit());
  lightObject->shader.setVec3("light.position",
     state->camera->cameraPos.x, state->camera->cameraPos.y, state->camera->cameraPos.z);
  lightObject->shader.setVec3("light.direction", 
    state->camera->cameraFront.x, state->camera->cameraFront.y, state->camera->cameraFront.z);
  lightObject->shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
  lightObject->shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

  state->specularMap->useTexture();
  lightObject->shader.setInt("material.specular", state->specularMap->getUnit());

  lightObject->shader.setVec3("dirLight.direction", state->lightPos);
  lightObject->shader.setVec3("dirLight.ambient", ambientColor);
  lightObject->shader.setVec3("dirLight.diffuse", diffuseColor);
  lightObject->shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

  lightObject->shader.setVec3("spotLight.position", state->camera->cameraPos);
  lightObject->shader.setVec3("spotLight.direction", state->camera->cameraFront);
  lightObject->shader.setVec3("spotLight.ambient", ambientColor);
  lightObject->shader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
  lightObject->shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  lightObject->shader.setFloat("spotLight.constant", 1.0f);
  lightObject->shader.setFloat("spotLight.linear", 0.09f);
  lightObject->shader.setFloat("spotLight.quadratic", 0.032f);
  lightObject->shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
  lightObject->shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)
   };

  for (int i = 0; i < 4; i++) {
    std::string location = "pointLight[" + std::to_string(i) + "]";
    lightObject->shader.setVec3((location + ".position").c_str(), pointLightPositions[i]);
    lightObject->shader.setVec3((location + ".ambient").c_str(), ambientColor);
    lightObject->shader.setVec3((location + ".diffuse").c_str(), diffuseColor);
    lightObject->shader.setVec3((location + ".specular").c_str(), glm::vec3(0.3f));
    lightObject->shader.setFloat((location + ".constant").c_str(), 1.0f);
    lightObject->shader.setFloat((location + ".linear").c_str(), 0.09f);
    lightObject->shader.setFloat((location + ".quadratic").c_str(), 0.032f);
  }

  for (unsigned int i = 0; i < 10; i++) {
    glm::mat4 model = glm::mat4(1.0f);  
    model = glm::translate(model, state->cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    Transform transform(model, view, projection);
    lightObject->updateTransform(transform);
    lightObject->useEntity();
  }

  // Transform transform3(glm::translate(glm::scale(model, glm::vec3(0.2f)),
  // state->lightPos), view, projection);

  for (int i = 0; i < 4; i++) {
    Transform transform3(
        glm::scale(glm::translate(model, pointLightPositions[i]), glm::vec3(0.2f)),
        view, projection);
    lightSource->updateTransform(transform3);
    lightSource->useEntity();
  }

  SDL_GL_SwapWindow(state->window);

  Uint64 currentTime = SDL_GetPerformanceCounter();
  state->deltaTime = currentTime - state->lastFrame;
  state->deltaSeconds = (float) state->deltaTime / SDL_GetPerformanceFrequency();
  state->lastFrame = currentTime;

  return SDL_APP_CONTINUE;
}