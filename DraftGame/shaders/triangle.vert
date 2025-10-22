#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec4 AreaColor;

void main() {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	vec4 lightPosition = vec4(3.0, 6.0, 0.0, 1.0);
	vec3 Kd = vec3(0.1, 0.1, 0.1);
	vec3 Ld = vec3(10.0, 10.0, 10.0);

	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = modelViewMatrix * vec4(VertexPosition, 1.0);
	vec3 s = normalize(vec3(lightPosition - eyeCoords));

	AreaColor = vec4(Ld * Kd * max(dot(s, tnorm), 0.0), 0.0); //vec4(VertexNormal, 0);
	gl_Position = projectionMatrix * modelViewMatrix * vec4(VertexPosition, 1.0);
}