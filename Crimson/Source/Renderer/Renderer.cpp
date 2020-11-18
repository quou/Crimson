#include "Renderer.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"

#include "Material.h"
#include "Renderer3D/LightScene.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	void GLErrorCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void *userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH: CR_LOG_FATAL_ERROR("%s", message); break;
			case GL_DEBUG_SEVERITY_MEDIUM: CR_LOG_ERROR("%s", message); break;
			case GL_DEBUG_SEVERITY_LOW: CR_LOG_WARNING("%s", message); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: CR_LOG("%s", message); break;
		}
	}

	void Renderer::Init() {
		CR_LOG("Using OpenGL version %s", glGetString(GL_VERSION));
		CR_LOG("Renderer: %s", glGetString( GL_RENDERER));

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLErrorCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_MULTISAMPLE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		instance().m_drawCallsCount = 0;

		instance().m_timeSinceStart = glfwGetTime();
	}

	void Renderer::ShadowPass(Camera& camera, LightScene& lightScene, std::vector<glm::mat4>& transforms, std::vector<Mesh*>& meshes) {
		int oldViewport[4];

		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glCullFace(GL_FRONT);

		lightScene.m_shadowmapShader->Bind();

		lightScene.BindShadowmapForWrite();

		int i = 0;
		for (auto& light : lightScene.m_directionalLights) {

			glViewport(i*1024,0,1024,1024);
			glScissor(i*1024,0,1024,1024);
			glEnable(GL_SCISSOR_TEST);
			glClear(GL_DEPTH_BUFFER_BIT);

			AABB sceneAABB{ glm::vec3(0.0f), glm::vec3(0.0f) };

			int ii = 0;
			for (auto mesh : meshes) {
				glm::vec3 position(transforms[ii][3]);
				AABB meshAABB = mesh->GetAABB();
				meshAABB.maxCorner += position;
				meshAABB.minCorner += position;

				if (meshAABB.minCorner.x < sceneAABB.minCorner.z) sceneAABB.minCorner.z = meshAABB.minCorner.x;
				if (meshAABB.minCorner.x > sceneAABB.maxCorner.x) sceneAABB.maxCorner.x = meshAABB.minCorner.x;
				if (meshAABB.minCorner.y < sceneAABB.minCorner.z) sceneAABB.minCorner.z = meshAABB.minCorner.y;
				if (meshAABB.minCorner.y > sceneAABB.maxCorner.y) sceneAABB.maxCorner.y = meshAABB.minCorner.y;
				if (meshAABB.minCorner.z < sceneAABB.minCorner.z) sceneAABB.minCorner.z = meshAABB.minCorner.z;
				if (meshAABB.minCorner.z > sceneAABB.maxCorner.z) sceneAABB.maxCorner.z = meshAABB.minCorner.z;

				ii++;
			}

			instance().m_sceneBound = sceneAABB;

			ii = 0;
			for (auto mesh : meshes) {
				lightScene.m_shadowmapShader->SetMat4("u_model", transforms[ii]);
				lightScene.m_shadowmapShader->SetMat4("u_directionalLightModel", light.CalculateTransform(sceneAABB));
				Draw(*mesh);

				ii++;
			}

			i++;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);
		glDisable(GL_SCISSOR_TEST);

		glViewport(0, 0, oldViewport[2], oldViewport[3]);
	}

	void Renderer::ShaderPass(Camera& camera, LightScene& lightScene, const glm::mat4& transform, Material& material) {
		if (material.m_albedo) {
			material.m_albedo->Bind(0);
		}
		lightScene.BindShadowmapForRead(1);

		material.m_shader->Bind();
		lightScene.Apply(instance().m_sceneBound, *material.m_shader);

		material.m_shader->SetInt("u_albedo", 0);
		material.m_shader->SetInt("u_directionalShadowmaps", 1);

		material.m_shader->SetVec3("u_cameraPosition", camera.position);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));

		material.m_shader->SetMat4("u_model", transform);
		material.m_shader->SetMat4("u_view", camera.GetView());
		material.m_shader->SetMat4("u_projection", camera.projection);
	}

	void Renderer::Draw(Mesh& mesh) {
		instance().m_drawCallsCount++;
		mesh.Draw();
	}

	void Renderer::EndFrame() {
		instance().m_updateTime = instance().m_timeSinceStart - instance().m_oldTimeSinceStart;
		instance().m_oldTimeSinceStart = instance().m_timeSinceStart;
	}
}
