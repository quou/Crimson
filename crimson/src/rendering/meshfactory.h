#pragma once

#include "core.h"
#include "mesh.h"
#include "memory.h"

namespace Crimson {
	class CR_API MeshFactory {
	public:
		static ref<Mesh> NewCubeMesh();
	};
}