#include <vector>
#include <string>
#include <sstream>
#include <string.h>

#include <glad/glad.h>

#include "assets.h"
#include "model.h"
#include "logger.h"
#include "renderdata.h"
#include "entity/components/lights.h"
#include "entity/components/transform.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace Crimson {
	Model::Model() {

	}

	Model::~Model() {

	}

	void Model::Draw(Shader* shader) {
		for (const ref<Mesh>& mesh : m_meshes) {
			shader->Bind();
			shader->SetUniformMat4("u_model", m_transform);
			mesh->DrawNoMaterial();
		}
	}

	void Model::DrawLitScene(const Camera& camera, Scene* scene) {
		for (const ref<Mesh>& mesh : m_meshes) {
			ref<Shader> s = AssetManager::LoadShader(mesh->m_material->m_shader.c_str());
			s->Bind();

			s->SetUniformVec3("u_cameraPosition", camera.position);

			SunComponent* sun = scene->GetSun();
			if (sun != NULL) {
				s->SetUniformBool("u_useSun", true);
				s->SetUniformVec3("u_sun.direction", sun->direction);
				s->SetUniformVec3("u_sun.color", sun->color);
				s->SetUniformFloat("u_sun.intensity", sun->intensity);
				s->SetUniformMat4("u_sun.transform", sun->GetTransform(camera));

				sun->BindShadowmap(0);
				s->SetUniformInt("u_shadowmap", 0);
			} else {
				s->SetUniformBool("u_useSun", false);
			}
			
			/* Apply point lights */
			for (unsigned int i = 0; i < scene->GetPointLights()->size(); i++) {
				PointLightComponent* light = scene->GetPointLights()->at(i);
				vec3 position = light->GetPosition();

				s->SetUniformVec3(("u_pointLights[" + std::to_string(i) + "].position").c_str(), position);
				s->SetUniformVec3(("u_pointLights[" + std::to_string(i) + "].color").c_str(), light->color);
				s->SetUniformFloat(("u_pointLights[" + std::to_string(i) + "].intensity").c_str(), light->intensity);
				s->SetUniformFloat(("u_pointLights[" + std::to_string(i) + "].constant").c_str(), light->constant);
				s->SetUniformFloat(("u_pointLights[" + std::to_string(i) + "].linear").c_str(), light->linear);
				s->SetUniformFloat(("u_pointLights[" + std::to_string(i) + "].quadratic").c_str(), light->quadratic);
			}

			/* Apply sky lights */
			for (unsigned int i = 0; i < scene->GetSkyLights()->size(); i++) {
				SkyLightComponent* light = scene->GetSkyLights()->at(i);

				s->SetUniformVec3(("u_skyLights[" + std::to_string(i) + "].color").c_str(), light->color);
				s->SetUniformFloat(("u_skyLights[" + std::to_string(i) + "].intensity").c_str(), light->intensity);
			}

			s->SetUniformInt("u_pointLightCount", scene->GetPointLights()->size());
			s->SetUniformInt("u_skyLightCount", scene->GetSkyLights()->size());

			s->SetUniformMat4("u_model", m_transform);
			s->SetUniformMat4("u_view", camera.GetView());
			s->SetUniformMat4("u_projection", camera.projection);
			s->SetUniformVec3("u_cameraPos", camera.position);

			mesh->Draw();
		}
	}

	void Model::AddMesh(const ref<Mesh>& mesh) {
		m_meshes.push_back(mesh);
	}
}
