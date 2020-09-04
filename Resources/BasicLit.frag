#version 330 core

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct Light {
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec3 cameraPosition;

uniform Material material;
uniform Light light;

void main() {
   vec3 norm = normalize(v_normal);

   // ambient
   vec3 ambient = light.ambient * material.ambient;

   // diffuse
   vec3 lightDirection = normalize(-light.direction);
   float diff = max(dot(norm, lightDirection), 0.0);
   vec3 diffuse = light.diffuse * (diff * material.diffuse);

   // specular
   vec3 viewDir = normalize(cameraPosition - v_fragPos);
   vec3 reflectDir = reflect(-lightDirection, norm);

   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   vec3 specular = light.specular * (spec * material.specular);

   vec3 lightingResult = (ambient + diffuse + specular);

   gl_FragColor = texture2D(tex, v_texCoord) * vec4(lightingResult, 1.0f);
}
