#version 430

const int MAX_POINT_LIGHTS			= 8;
const int MAX_SPOT_LIGHTS			= 8;
const int MAX_DIRECTIONAL_LIGHTS	= 2;
const float MAIN_AMBIENT			= 0.06;

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoords;

out vec4 LightIntensity;
out vec2 TexCoord;

struct DirectionalLight {
	vec4 position;
	vec3 direction;
};

struct PointLight {
	vec4 position;
	vec4 color;
};

struct SpotLight {
	vec4 position;
	vec4 color;
	vec4 direction;
};

layout(std140, binding = 0) uniform LightData {
	unsigned int directionalCount;
	unsigned int pointCount;
	unsigned int spotCount;

	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];
	DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
} lightData;

layout(std140, binding = 1) uniform MaterialData {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float sh;
} material;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform mat3 uNormalMatrix;

vec4 ads(int lightIndex, vec4 eyeCoords, vec3 tnorm) {
	vec3 toLight = vec3(lightData.pointLights[lightIndex].position - eyeCoords);
	float dist = length(toLight);

	vec3 s = toLight / max(dist, 1e-5);
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);

	float range = lightData.pointLights[lightIndex].color.a;
	float att = clamp(1.0 - (dist * dist) / (range * range), 0.0, 1.0);

	vec3 base = vec3(lightData.pointLights[lightIndex].color) * lightData.pointLights[lightIndex].position.w;
	vec3 ambient = base * material.ka;

	float sDotN = max(dot(s, tnorm), 0.0);
	vec3 diffuse = base * material.kd * sDotN * att;
	
	vec3 specular = vec3(0.0);
	if (sDotN > 0.0) {
		specular = base * material.ks * pow(max(dot(r, v), 0.0), material.sh) * att;
	}

	return vec4(ambient + diffuse + specular, 0.0);
}

void main() {
	mat4 viewModelMatrix = uViewMatrix * uModelMatrix;
	vec3 tnorm = normalize(uNormalMatrix * VertexNormal);
	vec4 eyeCoords = viewModelMatrix * vec4(VertexPosition, 1.0);

	LightIntensity = vec4(0.0, 0.0, 0.0, 1.0);
	for (int i = 0; i < lightData.pointCount; i++) {
		LightIntensity += ads(i, eyeCoords, tnorm);
	}

	TexCoord = VertexTexCoords;
	gl_Position = uProjectionMatrix * eyeCoords;
}