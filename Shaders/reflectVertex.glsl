#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float time;
uniform float speed;
uniform float amount;
uniform float height; 

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out Vertex{
   vec3 colour;
   vec2 texCoord;
   vec3 normal;
   vec3 worldPos;
}OUT;

void main(void) {
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

    OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	
    OUT.normal = normalize(normalMatrix * normalize(normal));

	float y = sin(time * speed + (position.x * position.z * amount) + 0.5 * cos(position.x * position.z * amount)) * height;

	vec4 worldPos = modelMatrix * vec4(vec3(position.x, position.y, position.z+y), 1);

    OUT.worldPos = worldPos.xyz;
    gl_Position = (projMatrix * viewMatrix) * worldPos;
}