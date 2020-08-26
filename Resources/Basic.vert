#version 330 core

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 view = mat4(1.0f);
uniform mat4 modl = mat4(1.0f);

void main() {
   v_normal = mat3(transpose(inverse(modl))) * normal;
   v_texCoord = texCoord;

   v_fragPos = (modl * vec4(position, 1.0)).xyz;

   gl_Position = view * modl * vec4(position, 1.0);
}
