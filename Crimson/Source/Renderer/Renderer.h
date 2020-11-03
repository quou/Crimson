#pragma once

#include <memory>

#include "Renderer3D/Mesh.h"
#include "Shader.h"

#include "Renderer3D/Camera.h"

namespace Crimson {

	class Material;
	class LightScene;

	class Renderer {
	private:
	public:
		static void Init();

		static void Clear();

		static void ShaderPass(const Camera& camera, LightScene& lightScene, const glm::mat4& transform, Material& material);
		static void Draw(Mesh& mesh);
	};

}
