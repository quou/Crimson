#pragma once

#include <glm/glm.hpp>

namespace Crimson {
	class Particle {
	private:
		glm::vec3 m_position;
		glm::vec3 m_velocity;
		float m_gravity;
		float m_lifetime;
		float m_rotation;
		float m_scale;

		float m_elapsedTime;

		friend class ParticleSystem;

	public:
		Particle(const glm::vec3& position, const glm::vec3& velocity, float lifetime, float gravity);

		bool Update(float delta);
	};
}
