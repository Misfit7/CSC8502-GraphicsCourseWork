#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex{
    vec2 texCoord;
}OUT;

void main(void) {
    vec4 worldPos = (modelMatrix * vec4(position, 1));

    gl_Position = (projMatrix * viewMatrix) * worldPos;


    OUT.texCoord = texCoord;
}