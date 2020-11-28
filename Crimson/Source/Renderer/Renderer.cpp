#include "Renderer.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"

#include "Material.h"
#include "Renderer3D/LightScene.h"
#include "DefaultAssets/StandardShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Crimson {
	void GLErrorCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void *userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH: CR_LOG_ERROR("%s", message); break;
			case GL_DEBUG_SEVERITY_MEDIUM: CR_LOG_WARNING("%s", message); break;
			case GL_DEBUG_SEVERITY_LOW: CR_LOG("%s", message); break;
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

		instance().m_wireframeShader = std::make_shared<Shader>(outlineShader);
	}

	void Renderer::Clear() {
		auto& i = instance();

		glClearColor(i.m_clearColor.x, i.m_clearColor.y, i.m_clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		i.m_drawCallsCount = 0;

		i.m_timeSinceStart = glfwGetTime();
	}

	void Renderer::ShadowPass(Camera& camera, LightScene& lightScene, std::vector<glm::mat4>& transforms, std::vector<Mesh*>& meshes) {
		int oldViewport[4];

		glGetIntegerv(GL_VIEWPORT, oldViewport);
		glCullFace(GL_FRONT);

		lightScene.m_shadowmapShader->Bind();

		lightScene.BindShadowmapForWrite();

		int i = 0;
		for (auto& light : lightScene.m_directionalLights) {

			int res = lightScene.m_shadowmapResolution;

			glViewport(0,0,res,res);
			glClear(GL_DEPTH_BUFFER_BIT);

			int ii = 0;
			for (auto mesh : meshes) {
				lightScene.m_shadowmapShader->SetMat4("u_model", transforms[ii]);
				lightScene.m_shadowmapShader->SetMat4("u_directionalLightModel", light.CalculateTransform(camera));
				Draw(*mesh);

				ii++;
			}

			i++;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);

		glViewport(0, 0, oldViewport[2], oldViewport[3]);
	}

	void Renderer::ShaderPass(Camera& camera, LightScene& lightScene, const glm::mat4& transform, Material& material) {
		if (material.m_albedo) {
			material.m_albedo->Bind(0);
		}
		lightScene.BindShadowmapForRead(1);

		material.m_shader->Bind();
		lightScene.Apply(camera, *material.m_shader);

		material.m_shader->SetInt("u_albedo", 0);
		material.m_shader->SetInt("u_directionalShadowmaps", 1);
		material.m_shader->SetInt("u_enviromentMap", 3);

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

	void Renderer::DrawWireframe(Camera& camera, const glm::mat4& transform, Mesh& mesh) {
		auto& m = instance();

		m.m_wireframeShader->Bind();
		m.m_wireframeShader->SetMat4("u_model", transform);
		m.m_wireframeShader->SetMat4("u_view", camera.GetView());
		m.m_wireframeShader->SetMat4("u_projection", camera.projection);
		m.m_wireframeShader->SetVec3("u_color", glm::vec3(0.0f, 1.0f, 0.0f));

		m.m_drawCallsCount++;

		mesh.DrawWireframe();
	}

	void Renderer::DeInit() {
		instance().m_wireframeShader.reset();
	}

	void Renderer::EndFrame() {
		instance().m_updateTime = (float)glfwGetTime() - instance().m_timeSinceStart;
	}

	void Renderer::SetClearColor(const glm::vec3& clearColor) {
		instance().m_clearColor = clearColor;
	}
}
