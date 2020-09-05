#version 330 core

#include Resources/Lights.glsl

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec3 cameraPosition;

uniform Material material;
uniform DirectionalLight directionalLight;


uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   vec3 ambient = light.ambient * material.ambient;


   vec3 lightDirection = normalize(-light.direction);
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);


   vec3 reflectDir = reflect(-lightDirection, normal);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
   vec3 lightDir = normalize(light.position - fragPos);
   // diffuse shading
   float diff = max(dot(normal, lightDir), 0.0);
   // specular shading
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   // attenuation
   float distance = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
   // combine results
   vec3 ambient = light.ambient * material.diffuse;
   vec3 diffuse = light.diffuse * diff * material.diffuse;
   vec3 specular = light.specular * spec * material.specular;
   ambient *= attenuation;
   diffuse *= attenuation;
   specular *= attenuation;
   return (diffuse + specular);
}

void main() {
   vec3 norm = normalize(v_normal);

   vec3 viewDir = normalize(cameraPosition - v_fragPos);

   vec3 lightingResult = CalculateDirectionalLight(directionalLight, norm, viewDir);
   for (int i = 0; i < pointLightCount; i++) {
      lightingResult += CalculatePointLight(pointLights[i], norm, v_fragPos, viewDir);
   }

   gl_FragColor = texture2D(tex, v_texCoord) * vec4(lightingResult, 1.0f);
}
