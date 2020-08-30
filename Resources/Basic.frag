#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

struct DirectionalLight {
   vec3 color;
   vec3 direction;
   float intensity;
};

struct AmbientLight {
   vec3 color;
   float intensity;
};

struct Material {
   float specularIntensity;
   float shininess;
};

uniform DirectionalLight directionalLight;
uniform AmbientLight ambientLight;
uniform Material material;

uniform vec3 eyePosition;

void main() {
   vec4 ambientColor = vec4(ambientLight.color, 1.0) * ambientLight.intensity;

   float diffuseFactor = max(dot(normalize(v_normal), normalize(directionalLight.direction)), 0.0);
   vec4 diffuseColor = vec4(directionalLight.color, 1.0) * directionalLight.intensity * diffuseFactor;

   vec4 specularColor = vec4(0,0,0,0);

   if (diffuseFactor > 0.0) {
      vec3 fragToEye = normalize(eyePosition - v_fragPos);
      vec3 reflectedNormal = normalize(-reflect(directionalLight.direction, normalize(v_normal)));

      float specularFactor = dot(fragToEye, reflectedNormal);
      if (specularFactor > 0.0) {
         specularFactor = pow(specularFactor, material.shininess);
         specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0);
      }
   }

   gl_FragColor = texture2D(tex, v_texCoord) * (ambientColor + diffuseColor + specularColor);
}
