#include "ParticleSystem.h"

#include "DefaultAssets/ParticleShader.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <ctime>

#include "Logger.h"

namespace Crimson {
	ParticleSystem::ParticleSystem(const glm::vec3& position) : m_position(position) {
		m_shader = std::make_shared<Shader>(ParticleShader);

		float vertices[] = {
           // pos      // tex
           0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 0.0f, 1.0f, 0.0f,
           0.0f, 0.0f, 0.0f, 0.0f,

           0.0f, 1.0f, 0.0f, 1.0f,
           1.0f, 1.0f, 1.0f, 1.0f,
           1.0f, 0.0f, 1.0f, 0.0f
       };

		 glGenVertexArrays(1, &m_quadVA);
       glGenBuffers(1, &m_quadVB);

       glBindBuffer(GL_ARRAY_BUFFER, m_quadVB);
       glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

       glBindVertexArray(m_quadVA);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
       glBindVertexArray(0);

		 srand (static_cast <unsigned> (time(0)));
	}

	static float RandomFloat(float min, float max) {
		return min + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(max-min)));
	}

	void ParticleSystem::Update(float delta) {


		auto it = m_particles.begin();
		while (it != m_particles.end()) {
			it->m_size -= m_sizeOverLifetime * delta;
			auto dead = it->Update(delta);
			if (dead) {
				it = m_particles.erase(it);
			} else {
				it++;
			}
		}

		for (int i = 0; i < m_rateOverTime; i++) {
			float rx = RandomFloat(m_randomVelocityMin, m_randomVelocityMax);
			float ry = RandomFloat(m_randomVelocityMin, m_randomVelocityMax);
			float rz = RandomFloat(m_randomVelocityMin, m_randomVelocityMax);
			float l = RandomFloat(m_randomLifetimeMin, m_randomLifetimeMax);

			if (m_particles.size() < m_maxParticles) {
				m_particles.push_back(Particle(m_position, glm::vec3(rx, ry, rz), l, m_gravity, m_startSize));
			}
		}
	}

	void ParticleSystem::Draw(Camera& camera) {
		for (auto& p : m_particles) {
			m_shader->Bind();

			glm::mat4 translation = glm::translate(glm::mat4(1.0f), p.m_position);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(p.m_size));
			glm::mat4 model = translation;

			glm::mat4 view = camera.GetView();

			model[0][0] = view[0][0];
			model[0][1] = view[1][0];
			model[0][2] = view[2][0];
			model[1][0] = view[0][1];
			model[1][1] = view[1][1];
			model[1][2] = view[2][1];
			model[2][0] = view[0][2];
			model[2][1] = view[1][2];
			model[2][2] = view[2][2];

			model *= scale;

			m_shader->SetMat4("u_model", model);
			m_shader->SetMat4("u_view", view);
			m_shader->SetMat4("u_projection", camera.projection);

			glCullFace(GL_FRONT);
			glBindVertexArray(m_quadVA);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			glCullFace(GL_BACK);
		}
	}
}
