#version 330 core

#include Resources/Shaders/Lights.glsl

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};


uniform sampler2D tex;
uniform sampler2D shadowMap;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;
in vec4 v_directionalLightPos;

uniform vec3 cameraPosition;

uniform Material material;
uniform DirectionalLight directionalLight;


uniform int pointLightCount;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

float ShadowCalculation(vec4 fragPosLightSpace)
{
   // perform perspective divide
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   // transform to [0,1] range
   projCoords = projCoords * 0.5 + 0.5;
   // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
   float closestDepth = texture(shadowMap, projCoords.xy).r;
   // get depth of current fragment from light's perspective
   float currentDepth = projCoords.z;
   // calculate bias (based on depth map resolution and slope)
   vec3 normal = normalize(v_normal);
   vec3 lightDir = normalize(directionalLight.direction - v_fragPos);
   float bias = max(0 * (1.0 - dot(normal, lightDir)), 0);
   // check whether current frag pos is in shadow
   // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
   // PCF
   float shadow = 0.0;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   for(int x = -1; x <= 1; ++x) {
      for(int y = -1; y <= 1; ++y)
      {
         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
         shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
      }
   }
   shadow /= 9.0;

   // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
   if(projCoords.z > 1.0)
      shadow = 0.0;

   return shadow;
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
   vec3 ambient = light.ambient * material.ambient;


   vec3 lightDirection = normalize(-light.direction);
   float diff = max(dot(normal, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);

   vec3 reflectDir = reflect(-lightDirection, normal);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   float shadow = ShadowCalculation(v_directionalLightPos);
   return (ambient + (1.0 - shadow) * (diffuse + specular));
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
   return (ambient + diffuse + specular);
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
