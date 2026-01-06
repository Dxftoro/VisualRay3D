#version 430

const int VR_RENDERER_MAX_LIGHTS = 10;

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

//layout(std140, binding = 2) uniform LightData {
//	int lightCount;
//	Light lights[VR_RENDERER_MAX_LIGHTS];
//} lightData;
//
layout(std140, binding = 0) uniform LightData {
	vec4 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
} light;

layout(std140, binding = 1) uniform MaterialData {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float sh;
} material;

//
//vec4 position = vec4(3.0, 15.0, 0.0, 1.0);
//vec3 la = vec3(0.03);
//vec3 ld = vec3(0.6);
//vec3 ls = vec3(1.0);
//vec3 ka = vec3(0.1);
//vec3 kd = vec3(1.0);
//vec3 ks = vec3(1.0);
//float sh = 64;
//
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main() {
	mat4 viewModelMatrix = viewMatrix * modelMatrix;
	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = viewModelMatrix * vec4(VertexPosition, 1.0);
	vec4 localLightPosition = viewMatrix * light.position;

	vec3 s = normalize(vec3(localLightPosition - eyeCoords));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);

	vec3 ambient = light.la * material.ka;

	float sDotN = max(dot(s, tnorm), 0.0);
	vec3 diffuse = light.ld * material.kd * sDotN;
	
	vec3 specular = vec3(0.0);
	if (sDotN > 0.0) {
		specular  = light.ls * material.ks * pow(max(dot(r, v), 0.0), material.sh);
	}

	LightIntensity = vec4(ambient + diffuse + specular, 1.0);
	TexCoord = VertexTexCoords;

	gl_Position = projectionMatrix * eyeCoords;
}