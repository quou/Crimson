#pragma once

#include <memory>

#include "Renderer3D/Mesh.h"
#include "Shader.h"

#include "Transform.h"
#include "Renderer3D/Camera.h"

namespace Crimson {

	class Renderer {
	private:
	public:
		Renderer();

		void Clear();

		void Draw(const Camera& camera, const Transform& transform, const std::shared_ptr<Shader>& shader, const std::shared_ptr<Mesh>& mesh);
	};

}
