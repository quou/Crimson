#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

uniform vec3 cameraPosition;

struct Material {
   vec3 color;
};

uniform Material material;

void main() {
   gl_FragColor = texture2D(tex, v_texCoord) * vec4(material.color, 1.0f);
}
