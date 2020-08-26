#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;

struct DirectionalLight {
   vec3 color;
   float ambientIntensity;
   vec3 direction;
   float diffuseIntensity;
};

uniform DirectionalLight directionalLight;

void main() {
   vec4 ambientColor = vec4(directionalLight.color, 1.0) * directionalLight.ambientIntensity;

   float diffuseFactor = max(dot(normalize(v_normal), normalize(directionalLight.direction)), 0.0);
   vec4 diffuseColor = vec4(directionalLight.color, 1.0) * directionalLight.diffuseIntensity * diffuseFactor;

   gl_FragColor = texture2D(tex, v_texCoord) * (ambientColor + diffuseColor);
}
