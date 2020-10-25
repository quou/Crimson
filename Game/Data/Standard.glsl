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

uniform int u_ambientLightCount = 0;
uniform int u_directionalLightCount = 0;

struct AmbientLight {
	float intensity;
	vec3 color;
};

struct DirectionalLight {
   vec3 direction;
	vec3 color;
	float intensity;
};

in vec2 v_texCoords;
in vec3 v_fragPos;
in vec3 v_normal;

uniform sampler2D u_albedo;

/* material */
uniform vec3 u_color = vec3(1);
uniform float u_smoothness = 1.0f;
uniform float u_shininess = 1.0f;

uniform vec3 u_cameraPosition;
uniform AmbientLight u_ambientLights[100];
uniform DirectionalLight u_directionalLights[100];

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   vec3 lightDirection = normalize(-light.direction);
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.color * (diff * u_color) * light.intensity;

   vec3 reflectDir = reflect(lightDirection, normal);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_shininess) * light.intensity;
   vec3 specular = light.color * (spec * u_color) * u_smoothness;

   return diffuse + specular;
}

void main() {
	vec3 norm = normalize(v_normal);
   vec3 viewDir = normalize(u_cameraPosition - v_fragPos);

	vec3 lightingResult = vec3(1);

	if (u_ambientLightCount > 0 || u_directionalLightCount > 0) {
		lightingResult = vec3(0);
	}

	for (int i = 0; i < u_ambientLightCount; i++) {
		lightingResult += u_ambientLights[i].color * vec3(u_ambientLights[i].intensity);
	}

	for (int i = 0; i < u_directionalLightCount; i++) {
		lightingResult += CalculateDirectionalLight(u_directionalLights[i], norm, viewDir);
	}


	gl_FragColor = texture2D(u_albedo, v_texCoords) * vec4(u_color, 1.0f) * vec4(lightingResult, 1.0f);
}

#end FRAGMENT
