#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec3 cameraPosition;

uniform float ambientStrength;
uniform vec3 ambientColor;

struct Material {
   vec3 color;
};

uniform Material material;

void main() {
   vec3 ambient = ambientStrength * ambientColor;

   vec3 lightingResult = material.color * ambient;

   gl_FragColor = texture2D(tex, v_texCoord) * vec4(lightingResult, 1.0f);
}
