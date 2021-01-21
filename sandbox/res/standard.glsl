#shader vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec2 v_uv;
out vec3 v_worldPos;
out vec3 v_normal;

void main() {
	v_uv = a_uv;
	v_normal = a_normal;
	v_worldPos = vec3(u_model * vec4(a_position, 1.0));

	gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
}

#shader pixel
#version 330 core

in vec2 v_uv;
in vec3 v_worldPos;
in vec3 v_normal;
out vec4 v_color;

struct Material {
	vec3 color;
	float shininess;
};

struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;

    float constant;
    float linear;
    float quadratic;
};

struct SkyLight {
	vec3 color;
	float intensity;
};

uniform vec3 u_cameraPosition;

uniform PointLight u_pointLights[100];
uniform int u_pointLightCount;

uniform SkyLight u_skyLights[5];
uniform int u_skyLightCount;

uniform Material u_material = Material(vec3(1.0, 0.0, 0.0), 32.0);

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - v_worldPos);
	float diff = max(dot(normal, lightDir), 0.0);
	
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 reflectDir = reflect(-lightDir, normal);
	
	float spec = pow(max(dot(normal, halfwayDir), 0.0), u_material.shininess) * light.intensity;
	
	float distance = length(light.position - v_worldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 diffuse = light.color * diff * u_material.color * light.intensity;
	vec3 specular = light.color * spec * u_material.color;
	
	diffuse *= attenuation;
	specular *= attenuation;
	
	return diffuse + specular;
}

void main() {
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(u_cameraPosition - v_worldPos);

	vec3 lightingResult = vec3(0.0);

	for (int i = 0; i < u_skyLightCount; i++) {
		lightingResult += u_material.color * u_skyLights[i].color * u_skyLights[i].intensity;
	}

	for (int i = 0; i < u_pointLightCount; i++) {
		lightingResult += CalculatePointLight(u_pointLights[i], normal, viewDir);
	}

	v_color = vec4(lightingResult, 1.0);
}
