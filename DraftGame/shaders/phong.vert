#version 430

const int VR_RENDERER_MAX_LIGHTS = 16;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

out vec4 LightIntensity;
out vec2 TexCoord;

struct Light {
	vec4 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
};

layout(std140, binding = 0) uniform LightData {
	int lightCount;
	Light lights[VR_RENDERER_MAX_LIGHTS];
} lightData;

layout(std140, binding = 1) uniform MaterialData {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float sh;
} material;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

vec4 ads(int lightIndex, vec4 eyeCoords, vec3 tnorm) {
	vec3 s = normalize(vec3(
		viewMatrix * lightData.lights[lightIndex].position - eyeCoords));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);

	vec3 ambient = lightData.lights[lightIndex].la * material.ka;

	float sDotN = max(dot(s, tnorm), 0.0);
	vec3 diffuse = lightData.lights[lightIndex].ld * material.kd * sDotN;
	
	vec3 specular = vec3(0.0);
	if (sDotN > 0.0) {
		specular = lightData.lights[lightIndex].ls 
			* material.ks 
			* pow(max(dot(r, v), 0.0), material.sh);
	}

	return vec4(ambient + diffuse + specular, 0.0);
}

void main() {
	mat4 viewModelMatrix = viewMatrix * modelMatrix;
	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = viewModelMatrix * vec4(VertexPosition, 1.0);

	LightIntensity = vec4(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < lightData.lightCount; i++) {
		LightIntensity += ads(i, eyeCoords, tnorm);
	}

	TexCoord = VertexTexCoords;
	gl_Position = projectionMatrix * eyeCoords;
}