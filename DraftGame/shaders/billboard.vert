#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in float aSize;

out float vSize;

void main() {
	gl_Position = vec4(aPosition, 1.0);
	vSize = aSize;
}