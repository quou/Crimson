#begin VERTEX

#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoords;

out vec2 v_texCoords;
out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	v_texCoords = vec2(a_texCoords.x, a_texCoords.y);
	v_fragPos = (u_model * vec4(a_pos, 1.0)).xyz;
	v_normal = mat3(transpose(inverse(u_model))) * a_normal;

	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 FragColor;

uniform int u_ambientLightCount = 0;
uniform int u_directionalLightCount = 0;
uniform int u_pointLightCount = 0;

uniform float u_gamma = 2.2;

struct AmbientLight {
	float intensity;
	vec3 color;
};

struct DirectionalLight {
   vec3 direction;
	vec3 color;
	float intensity;

	mat4 transform;
	mat4 transformLightSpace;
	int index;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 color;
	float intensity;
};

in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;

uniform sampler2D u_albedo;
uniform sampler2D u_directionalShadowmaps;

struct Material {
	vec3 color;
	float smoothness;
	float shininess;
};

uniform vec3 u_cameraPosition;
uniform AmbientLight u_ambientLights[100];
uniform DirectionalLight u_directionalLights[10];
uniform PointLight u_pointLights[100];

uniform Material u_material;

float CalculateDirectionalShadow(DirectionalLight light) {
	vec4 lightSpacePos = light.transform * vec4(v_fragPos, 1.0);
	vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;

	vec2 texPos = projCoords.xy;

	float widthPixel = 1.0f / (1024.0 * 10.0);
	float heightPixel = 1.0f / 1024.0;

	vec4 source = vec4(1024.0 * light.index, 0.0, 1024.0, 1024.0);

	float startX = source.x, startY = source.y, width = source.z, height = source.w;
	vec2 coords = vec2(widthPixel * startX + width * widthPixel * texPos.x, heightPixel * startY + height * heightPixel * texPos.y);

	float closestDepth = texture(u_directionalShadowmaps, coords).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	if (projCoords.z > 1.0 || projCoords.x >= 1.0 || projCoords.y >= 1.0) {
		shadow = 0.0;
	}

	return shadow;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   vec3 lightDirection = normalize(-light.direction);
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.color * (diff * u_material.color) * light.intensity;

   vec3 reflectDir = reflect(-lightDirection, normal);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess) * light.intensity;
   vec3 specular = light.color * (spec * u_material.color) * u_material.smoothness;

   return (1.0 - CalculateDirectionalShadow(light)) * (diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - v_fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess) * light.intensity;

	float distance = length(light.position - v_fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 diffuse = light.color * diff * u_material.color * light.intensity;
   vec3 specular = light.color * spec * u_material.color * u_material.smoothness;
   diffuse *= attenuation;
   specular *= attenuation;

	return diffuse + specular;
}

void main() {
	vec3 norm = normalize(v_normal);
   vec3 viewDir = normalize(u_cameraPosition - v_fragPos);

	vec3 lightingResult = vec3(1);

	if (u_ambientLightCount > 0 || u_directionalLightCount > 0 || u_pointLightCount > 0) {
		lightingResult = vec3(0);
	}

	for (int i = 0; i < u_ambientLightCount; i++) {
		lightingResult += u_ambientLights[i].color * vec3(u_ambientLights[i].intensity);
	}

	for (int i = 0; i < u_directionalLightCount; i++) {
		lightingResult += CalculateDirectionalLight(u_directionalLights[i], norm, viewDir);
	}

	for (int i = 0; i < u_pointLightCount; i++) {
		lightingResult += CalculatePointLight(u_pointLights[i], norm, viewDir);
	}

	vec3 diffuseColor = pow(texture(u_albedo, v_texCoords).rgb, vec3(u_gamma));

	FragColor = vec4(diffuseColor, 1.0f) * vec4(u_material.color, 1.0f) * vec4(lightingResult, 1.0f);

	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/u_gamma));
}

#end FRAGMENT
