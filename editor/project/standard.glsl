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
	v_worldPos = (u_model * vec4(a_position, 1.0)).xyz;

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

struct Sun {
	vec3 direction;
	vec3 color;
	float intensity;

	mat4 transform;
};

uniform vec3 u_cameraPosition;

uniform PointLight u_pointLights[100];
uniform int u_pointLightCount;

uniform SkyLight u_skyLights[5];
uniform int u_skyLightCount;

uniform bool u_useSun;
uniform Sun u_sun;
uniform sampler2D u_shadowmap;

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

float CalculateDirectionalShadow(Sun light, vec3 normal, vec3 lightDir) {
	vec4 lightSpacePos = light.transform * vec4(v_worldPos, 1.0);
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closestDepth = texture(u_shadowmap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	
	float bias = max(0.00005 * (1.0 - dot(normal, lightDir)), 0.000005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_shadowmap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(u_shadowmap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	
	if (projCoords.z > 1.0) {
		shadow = 0.0;
	}
	return shadow;
}

vec3 CalculateSun(Sun light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
	
    vec3 diffuse = light.color * diff * u_material.color * light.intensity;
    vec3 specular = light.color * spec * u_material.color * light.intensity;

    return (1.0 - CalculateDirectionalShadow(light, normal, lightDir)) * (diffuse + specular);
}

void main() {
	vec3 normal = normalize(v_normal);
	vec3 viewDir = normalize(u_cameraPosition - v_worldPos);

	vec3 lightingResult = vec3(0.0);

	for (int i = 0; i < u_skyLightCount; i++) {
		lightingResult += u_material.color * u_skyLights[i].color * u_skyLights[i].intensity;
	}

	lightingResult += CalculateSun(u_sun, normal, viewDir);

	for (int i = 0; i < u_pointLightCount; i++) {
		lightingResult += CalculatePointLight(u_pointLights[i], normal, viewDir);
	}

	v_color = vec4(lightingResult, 1.0);
}
