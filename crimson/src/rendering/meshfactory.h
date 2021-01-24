#pragma once

#include "core.h"
#include "mesh.h"
#include "memory.h"

namespace Crimson {
	class CR_API MeshFactory {
	private:
	public:
		static ref<Mesh> NewCubeMesh(const ref<Material>& material);
		static ref<Mesh> NewSphereMesh(const ref<Material>& material);
	};
}
