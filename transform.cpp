
#include "headers/transform.hpp"
#include <GLES3/gl31.h>
#include "external/glm/gtc/type_ptr.hpp"

Transform::Transform(glm::mat4 model, glm::mat4 view, glm::mat4 projection) : model(model), view(view), projection(projection) {}

Transform::~Transform() {}

void Transform::applyTransform(const Shader &shader) const {
    int modelLocation = glGetUniformLocation(shader.getProgram(), shader.getUniformLocations().modelLocation);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    int viewLocation = glGetUniformLocation(shader.getProgram(), shader.getUniformLocations().viewLocation);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLocation = glGetUniformLocation(shader.getProgram(), shader.getUniformLocations().projectionLocation);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

}


