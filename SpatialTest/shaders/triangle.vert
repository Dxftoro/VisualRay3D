#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;

out vec4 AreaColor;
out vec2 TexCoord;

void main() {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	vec4 llightPosition = viewMatrix * vec4(3.0, 15.0, 0.0, 1.0);
	vec3 Kd = vec3(0.1, 0.1, 0.1);
	vec3 Ld = vec3(10.0, 10.0, 10.0);

	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = modelViewMatrix * vec4(VertexPosition, 1.0);
	vec3 s = normalize(vec3(llightPosition - eyeCoords));

	AreaColor = vec4(Ld * Kd * max(dot(s, tnorm), 0.0), 0.0);
	TexCoord = VertexTexCoords;
	gl_Position = projectionMatrix * eyeCoords;
}