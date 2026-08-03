
#version 310 es
precision mediump float;

in vec3 color;
in vec2 uv;

out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, uv);
}