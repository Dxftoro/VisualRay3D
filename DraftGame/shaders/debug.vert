#version 410

layout (location = 0) in vec3 VertexPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main() {
	gl_Position = projectionMatrix * viewMatrix * vec4(VertexPosition, 1.0);
}