
#pragma once

#include "external/glm/mat4x4.hpp"
#include "external/glm/vec3.hpp"

class Camera {

    public:
        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

        float cameraSpeed;

        float pitch = 0.0f;
        float yaw = -90.0f;

        glm::mat4 getViewMatrix();

        Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed);

        ~Camera();

    private:

};