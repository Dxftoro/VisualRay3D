#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 VertexTexCoords;

out vec3 LightIntensity;
out vec3 TexCoord;

struct LightData {
	vec4 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
};

struct MaterialData {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float sh;
};

uniform LightData light;
uniform MaterialData material;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main() {
	mat4 viewModelMatrix = viewMatrix * modelMatrix;
	vec3 tnorm = normalize(normalMatrix * VertexNormal);
	vec4 eyeCoords = viewModelMatrix * vec4(VertexPosition, 1.0);

	vec3 s = normalize(vec3(light.position - eyeCoords));
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);

	vec3 ambient = light.la * material.ka;

	float sDotN = max(dot(s, tnorm), 0.0);
	vec3 diffuse = light.ld * material.kd * sDotN;
	
	vec3 specular = vec3(0.0);
	if (sDotN > 0.0) {
		specular  = light.ls * material.ks * pow(max(dot(r, v), 0.0), material.sh);
	}

	LightIntensity = ambient + diffuse + specular;
	TexCoord = VertexTexCoords;
	gl_Position = projectionMatrix * viewModelMatrix * vec4(VertexPosition, 1.0);
}