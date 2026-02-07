#version 430

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

out vec2 TexCoords;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uCameraPosition;

void main() {
	vec3 current = gl_in[0].gl_Position.xyz;
	vec3 toPoint = normalize(current - uCameraPosition);
	vec3 worldUp = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(toPoint, worldUp));
	vec3 up = normalize(cross(right, toPoint));
	
	mat4 projectionView = uProjectionMatrix * uViewMatrix;
	float size = 3.0;

	gl_Position = projectionView * vec4(current - (right - up) * size, 1.0);
	TexCoords = vec2(0.0, 0.0);
	EmitVertex();

	gl_Position = projectionView * vec4(current - (right + up) * size, 1.0);
	TexCoords = vec2(0.0, 1.0);
	EmitVertex();

	gl_Position = projectionView * vec4(current + (right + up) * size, 1.0);
	TexCoords = vec2(1.0, 0.0);
	EmitVertex();

	gl_Position = projectionView * vec4(current + (right - up) * size, 1.0);
	TexCoords = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}