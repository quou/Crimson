#pragma once

#include <Crimson.h>

#include "EditorLayer.h"

#include "SceneCamera.h"

static const char* outlineShader = R"(
#begin VERTEX
#version 330 core

layout (location = 0) in vec3 a_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
	gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
}

#end VERTEX

#begin FRAGMENT
#version 330 core

out vec4 a_fragColor;

uniform vec3 u_color;

void main() {
	a_fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

#end FRAGMENT
)";

class Editor : public Crimson::Game {
public:
	std::shared_ptr<Crimson::Scene> m_scene;

	std::shared_ptr<Crimson::RenderTarget> m_gameRenderTarget;
	std::shared_ptr<Crimson::RenderTarget> m_sceneRenderTarget;

	std::shared_ptr<EditorLayer> m_editorLayer;

	std::shared_ptr<Crimson::Shader> m_wireFrameShader;

	Crimson::Entity cam;
	Crimson::Entity mainLight;

	SceneCamera m_camera;
private:
	void OnInit() override {
		Crimson::Input::RegisterKey("shift", CR_KEY_LEFT_SHIFT);

		m_scene = std::make_shared<Crimson::Scene>(false);

		m_sceneRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());
		m_gameRenderTarget = std::make_shared<Crimson::RenderTarget>(GetWindowSize());

		m_wireFrameShader = std::make_shared<Crimson::Shader>(outlineShader);

		m_editorLayer = AddLayer<EditorLayer>(this, &m_camera, m_sceneRenderTarget.get(), m_gameRenderTarget.get(), m_scene.get());
	}

	void OnUpdate(float delta) override {
		m_camera.UpdateViewport(m_sceneRenderTarget->GetSize());

		m_scene->UpdateViewport(m_sceneRenderTarget->GetSize());
		m_scene->Render(*m_sceneRenderTarget, m_camera.GetCamera(), delta);

		if (m_editorLayer->m_sceneHierarchyPanel.m_selectedEntity) {
			if (m_editorLayer->m_sceneHierarchyPanel.m_selectedEntity.HasComponent<Crimson::MeshFilterComponent>()) {
				auto mc = m_editorLayer->m_sceneHierarchyPanel.m_selectedEntity.GetComponent<Crimson::MeshFilterComponent>();
				auto mesh = m_scene->m_assetManager.LoadMesh(mc.path);
				if (mesh) {
					Crimson::Renderer::ClearDepth();
					Crimson::Renderer::DrawWireframe(*m_camera.GetCamera(), m_editorLayer->m_sceneHierarchyPanel.m_selectedEntity.GetComponent<Crimson::TransformComponent>().GetTransform(), *mesh);
				}
			}
		}

		m_sceneRenderTarget->Unbind();

		m_scene->UpdateViewport(m_gameRenderTarget->GetSize());
		m_scene->Render(*m_gameRenderTarget, delta);
		m_gameRenderTarget->Unbind();
   }

	void OnExit() override {
		m_wireFrameShader.reset();
		m_sceneRenderTarget.reset();
		m_gameRenderTarget.reset();
		m_scene.reset();
		m_editorLayer.reset();
	}
public:
};
