#version 330 core

in vec3 v_texCoord;

uniform samplerCube skybox;

void main() {
   gl_FragColor = texture(skybox, v_texCoord);
}
