#shader vertex
#version 330 core

uniform vec4 u_thing = vec4(1.0f);

struct Material {
	int hi;
	int something;
};

uniform Material u_material = Material(0, 1);

void main() {

}

#shader pixel
#version 330 core

void main() {

}
