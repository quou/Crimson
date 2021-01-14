#pragma once

#include "core.h"
#include "mesh.h"
#include "memory.h"
#include "camera.h"
#include "math/mat4.h"

namespace Crimson {
	class CR_API Model {
	private:
		std::vector<ref<Mesh>> m_meshes;

		mat4 m_transform;
	public:
		Model();
		virtual ~Model();

		void AddMesh(const ref<Mesh>& mesh);
	
		inline void SetTransform(const mat4& t) { m_transform = t; }

		void Draw(const Camera& camera);
	};
}
