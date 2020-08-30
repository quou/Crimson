#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

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
