
#include "headers/camera.hpp"
#include "external/glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp, float cameraSpeed) 
    : cameraPos(cameraPos), cameraFront(cameraFront), cameraUp(cameraUp), cameraSpeed(cameraSpeed)
    
    {


    }

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    glm::mat4 lookAt = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    return lookAt;
}
