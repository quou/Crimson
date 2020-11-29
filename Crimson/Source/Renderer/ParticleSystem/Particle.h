#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	class Particle {
	private:
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		float m_gravity{0.5f};
		float m_lifetime{1.0f};
		float m_size{1.0f};

		float m_elapsedTime;

		friend class ParticleSystem;

	public:
		Particle(const glm::vec3& position, const glm::vec3& velocity, float lifetime, float gravity, float size);

		bool Update(float delta);
	};
}
