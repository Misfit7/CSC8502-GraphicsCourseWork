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
in vec4 colour;
in vec3 normal;
in vec4 tangent;
in vec2 texCoord;

out Vertex {
  vec3 position;
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent; 
  vec3 binormal;
  vec3 worldPos;
}
OUT;

void main()
{
	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec3 wNormal = normalize(normalMatrix * normalize(normal));
	vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
	
	OUT.normal = wNormal;
	OUT.tangent = wTangent;
	OUT.binormal = cross(wTangent, wNormal) * tangent.w;

	float xx = (position.x-3) * (position.x-3);
	float yy = (position.y+1) * (position.y+1);
	float y = sin(time * speed + (position.x * position.z * amount) + 0.5 * cos(position.x * position.z * amount)) * height;

	vec4 worldPos = modelMatrix * vec4(vec3(position.x, y, position.z), 1);
	OUT.worldPos = worldPos.xyz;

	gl_Position = (projMatrix * viewMatrix) * worldPos;
}