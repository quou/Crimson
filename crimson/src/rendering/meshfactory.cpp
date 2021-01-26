#include <math.h>

#include "meshfactory.h"
#include "logger.h"
#include "math/maths.h"

namespace Crimson {
	ref<Mesh> MeshFactory::NewCubeMesh(const ref<Material>& material) {
		ref<Mesh> cube(new Mesh(
			std::vector<Vertex> {
				{vec3(1.0f, 1.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.625, 0.500)},
				{vec3(-1.0f, 1.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.875, 0.500)},
				{vec3(-1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.875, 0.750)},
				{vec3(1.0f, 1.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.625, 0.500)},
				{vec3(-1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.875, 0.750)},
				{vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), vec2(0.625, 0.750)},
				{vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.375, 0.750)},
				{vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.625, 0.750)},
				{vec3(-1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.625, 1.0f)},
				{vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.375, 0.750)},
				{vec3(-1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.625, 1.0f)},
				{vec3(-1.0f, -1.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f), vec2(0.375, 1.0f)},
				{vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.375, 0.0f)},
				{vec3(-1.0f, 1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.625, 0.0f)},
				{vec3(-1.0f, 1.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.625, 0.250)},
				{vec3(-1.0f, -1.0f, 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.375, 0.0f)},
				{vec3(-1.0f, 1.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.625, 0.250)},
				{vec3(-1.0f, -1.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec2(0.375, 0.250)},
				{vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.125, 0.500)},
				{vec3(1.0f, -1.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.375, 0.500)},
				{vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.375, 0.750)},
				{vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.125, 0.500)},
				{vec3(1.0f, -1.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.375, 0.750)},
				{vec3(-1.0f, -1.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f), vec2(0.125, 0.750)},
				{vec3(1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.375, 0.500)},
				{vec3(1.0f, 1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.625, 0.500)},
				{vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.625, 0.750)},
				{vec3(1.0f, -1.0f, -1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.375, 0.500)},
				{vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.625, 0.750)},
				{vec3(1.0f, -1.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec2(0.375, 0.750)},
				{vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.375, 0.250)},
				{vec3(-1.0f, 1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.625, 0.250)},
				{vec3(1.0f, 1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.625, 0.500)},
				{vec3(-1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.375, 0.250)},
				{vec3(1.0f, 1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.625, 0.500)},
				{vec3(1.0f, -1.0f, -1.0f), vec3(0.0f, 0.0f, -1.0f), vec2(0.375, 0.500)},
			},
			std::vector<unsigned int> {
				0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
			}, material
		));

		cube->m_instanceType = Mesh::INSTANCE;
		cube->m_factoryType = Mesh::CUBE;

		return cube;
	}

	ref<Mesh> MeshFactory::NewSphereMesh(const ref<Material>& material) {
		/* Algorithm courtesy of http://www.songho.ca/opengl/gl_sphere.html */

		float radius = 1.0f;
		float sectorCount = 36.0f;
		float stackCount = 18.0f;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		float x, y, z, xy;
		float nx, ny, nz, lengthInv = 1.0f / radius;
		
		float s, t;
		
		float sectorStep = 2 * CR_PI / sectorCount;
		float stackStep = CR_PI / stackCount;
		float sectorAngle, stackAngle;

		for(int i = 0; i <= stackCount; ++i) {	
			int k1 = i * (sectorCount + 1);
			int k2 = k1 + sectorCount + 1;


			stackAngle = CR_PI / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for(int j = 0; j <= sectorCount; ++j, ++k1, ++k2) {
				sectorAngle = j * sectorStep;

				Vertex vertex;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				vertex.position = vec3(x, y, z);

				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertex.normal = vec3(nx, ny, nz);

				s = (float)j / sectorCount;
				t = (float)i / stackCount;
				vertex.uv = vec2(s, t);

				vertices.push_back(vertex);


				if (i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}


				if (i != (stackCount-1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}

		ref<Mesh> sphere(new Mesh(vertices, indices, material));

		sphere->m_instanceType = Mesh::INSTANCE;
		sphere->m_factoryType = Mesh::SPHERE;
		
		return sphere;
	}
}
