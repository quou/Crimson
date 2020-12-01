#include "Particle.h"


namespace Crimson {
	Particle::Particle(const glm::vec3& position, const glm::vec3& velocity, float lifetime, float gravity, float size) :
		m_position(position), m_velocity(velocity), m_lifetime(lifetime), m_gravity(gravity), m_size(size) {}

	bool Particle::Update(float delta) {
		m_velocity.y -= m_gravity;
		m_position += m_velocity * delta;

		m_elapsedTime += delta;
		return m_elapsedTime > m_lifetime;
	}
}
