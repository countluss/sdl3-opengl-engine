
#include "headers/shader.hpp"
#include "headers/opengles.hpp"
#include "headers/uniform_locations.hpp"
#include <GLES3/gl31.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath):

    Shader(vertexShaderPath, fragmentShaderPath,
        UniformLocations(

            "uTexture", "uModel", "uView", "uProjection"

        ))

    {};

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath,
    const UniformLocations uniformLocations
): uniformLocations(uniformLocations)
     {

    std::string vertexShaderSource = readFile(vertexShaderPath);
    std::string fragmentShaderSource = readFile(fragmentShaderPath);

    const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
    const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();

    compileAndLinkShaders(vertexShaderSourceCStr, fragmentShaderSourceCStr);

}

void Shader::compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {

    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error: Vertex Shader Compilation Failed\n" << infoLog << std::endl;

        return;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error: Fragment Shader Compilation Failed\n"
                  << infoLog << std::endl;

        return;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Error: Program Linking Failed\n"
                  << infoLog << std::endl;
        return;
    }

    
    // Delete individual shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    std::cout << "Shader Compiled and Linked Successfully\n";
    status = true;

}

void Shader::useProgram() const {
    glUseProgram(program);
}

bool Shader::getShaderStatus() const {
    return status;
}

Shader::~Shader() {

    if (vertexShader != 0)
        glDeleteShader(vertexShader);
    if (fragmentShader != 0)
        glDeleteShader(fragmentShader);
    if (program != 0)
        glDeleteProgram(program);

    if (vertexShader != 0 || fragmentShader != 0 || program != 0)
        std::cout << "Destroying Shader\n";
}

std::string Shader::readFile(std::string filePath) {
  std::ifstream shaderFile(filePath);

  if (!shaderFile.is_open()) {
    std::cerr << "Error opening shader file: " << filePath << std::endl;
    return "";
  }

  std::stringstream shaderStream;
  std::string shaderCode;

  shaderStream << shaderFile.rdbuf();
  shaderFile.close();

  shaderCode = shaderStream.str();

  return shaderCode;
}

unsigned int Shader::getProgram() const {
    return program;
}

const UniformLocations Shader::getUniformLocations() const {
    return uniformLocations;
}

void Shader::setVec3(const char* name, float x, float y, float z) {
    int uniform = glGetUniformLocation(getProgram(), name);
    glUniform3f(uniform, x, y, z);
}

void Shader::setFloat(const char* name, float value) {
    int uniform = glGetUniformLocation(getProgram(), name);
    glUniform1f(uniform, value);
}

void Shader::setInt(const char* name, int value) {
    int uniform = glGetUniformLocation(getProgram(), name);
    glUniform1i(uniform, value);
}

void Shader::setVec3(const char* name, glm::vec3 vector) {
    int uniform = glGetUniformLocation(getProgram(), name);
    glUniform3f(uniform, vector.x, vector.y, vector.z);
}