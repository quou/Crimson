#pragma once

#include <vector>

#include "Renderer/Vertex.h"

namespace Crimson {

	class Mesh {
	private:
		unsigned int m_va, m_vb, m_ib;
	public:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		Mesh(const char* obj);

		void LoadFromData(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		void LoadFromWavefront(const char* obj);

		void Draw();

		~Mesh();
	};

}
