#pragma once

#include <vector>

#include "core.h"
#include "renderdata.h"
#include "memory.h"
#include "material.h"

namespace Crimson {
	class CR_API Mesh {
	private:
		unsigned int m_va, m_vb, m_ib;

		unsigned int m_indexCount;

		ref<Material> m_material;

		friend class Model;
	public:
		Mesh(const std::vector<Vertex>& vertices, 
				const std::vector<unsigned int>& indices, const ref<Material>& material);

		inline void UseMaterial(const ref<Material>& material) { m_material = material; }
		
		virtual ~Mesh();

		void LoadFromVertexData(const std::vector<Vertex>& vertices,
				const std::vector<unsigned int>& indices);

		void Draw();
	};
}
