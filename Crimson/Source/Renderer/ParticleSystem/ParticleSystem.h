#pragma once

#include <memory>
#include <vector>

#include "Particle.h"

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Renderer3D/Camera.h"

namespace Crimson {
	class ParticleSystem {
	private:
		unsigned int m_quadVA;
		unsigned int m_quadVB;

		std::shared_ptr<Shader> m_shader;

		std::vector<Particle> m_particles;
	public:
		int m_maxParticles{100};
		glm::vec3 m_position;
		float m_rateOverTime;

		ParticleSystem(glm::vec3 position, Surface* texture);

		void Update(float delta);
		void Draw(Camera& camera);
	};
}
