#version 330 core

uniform sampler2D tex;

in vec2 v_texCoord;

void main() {
   gl_FragColor = texture2D(tex, v_texCoord);
}
