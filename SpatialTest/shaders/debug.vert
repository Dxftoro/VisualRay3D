#version 410

layout (location = 0) in vec3 VertexPosition;

uniform mat4 projectionViewMatrix;

void main() {
	gl_Position = projectionViewMatrix * vec4(VertexPosition, 1.0);
}