#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec3 cameraPosition;

uniform float ambientStrength;
uniform vec3 ambientColor;

uniform vec3 directionalLightPos;

struct Material {
   vec3 color;
   float specularStrength;
};

uniform Material material;

void main() {
   vec3 norm = normalize(v_normal);
   vec3 lightDirection = normalize(directionalLightPos - v_fragPos);

   float diff = max(dot(norm, lightDirection), 0.0);
   vec3 diffuse = diff * ambientColor;

   vec3 viewDir = normalize(cameraPosition - v_fragPos);
   vec3 reflectDir = reflect(-lightDirection, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = material.specularStrength * spec * ambientColor;

   vec3 ambient = ambientStrength * ambientColor;

   vec3 lightingResult = (ambient + diffuse + specular) * material.color;

   gl_FragColor = texture2D(tex, v_texCoord) * vec4(lightingResult, 1.0f);
}
