#version 310 es

precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 uv;
out vec3 normal;
out vec3 pos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormal;

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    color = aColor;
    uv = aUV;
    pos = vec3(uModel * vec4(aPos, 1.0));
    normal = uNormal * aNormal;

}