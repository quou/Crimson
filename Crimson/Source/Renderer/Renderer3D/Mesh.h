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

		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		Mesh(const char* lua);

		void LoadFromData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void LoadFromLua(const char* lua);

		void Draw();

		~Mesh();
	};

}
