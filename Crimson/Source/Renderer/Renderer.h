#pragma once

#include <memory>

#include "Renderer3D/Mesh.h"
#include "Shader.h"

namespace Crimson {

	class Renderer {
	private:
	public:
		Renderer();

		void Clear();

		void DrawMesh(std::shared_ptr<Shader> shader, std::shared_ptr<Mesh> mesh);
	};

}
