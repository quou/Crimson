#pragma once

#include "core.h"
#include "mesh.h"
#include "memory.h"

namespace Crimson {
	class CR_API Model {
	private:
		std::vector<ref<Mesh>> m_meshes;
	public:
		Model();
		virtual ~Model();

		void AddMesh(const ref<Mesh>& mesh);
	
		void Draw();
	};
}
