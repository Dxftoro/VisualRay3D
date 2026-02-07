#version 430

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

out vec2 TexCoords;

uniform mat4 uProjectionMatrix;
uniform vec3 uCameraPosition;

void main() {
	vec3 current = gl_in[0].gl_Position.xyz;

	vec3 toCamera = normalize(uCameraPosition - current);
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(toCamera, up);

	current -= (right * 0.5);
	gl_Position = uProjectionMatrix * vec4(current, 1.0);
	TexCoords = vec2(0.0, 0.0);
	EmitVertex();

	current += 1.0;
	gl_Position = uProjectionMatrix * vec4(current, 1.0);
	TexCoords = vec2(0.0, 1.0);
	EmitVertex();

	current -= 1.0;
	gl_Position = uProjectionMatrix * vec4(current, 1.0);
	TexCoords = vec2(1.0, 0.0);
	EmitVertex();

	current += 1.0;
	gl_Position = uProjectionMatrix * vec4(current, 1.0);
	TexCoords = vec2(1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}