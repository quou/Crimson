#include "Skybox.h"

#include <glad/glad.h>

namespace Crimson {
	static const char* skyboxShader = R"(
#begin VERTEX

#version 330

layout (location = 0) in vec3 a_pos;

out vec3 a_texCoords;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
	a_texCoords = a_pos;
	gl_Position = u_projection * u_view * vec4(a_pos, 1.0f);
}

#end VERTEX

#begin FRAGMENT

#version 330 core

out vec4 a_fragColor;

in vec3 a_texCoords;

uniform samplerCube u_cubemap;

void main() {
	a_fragColor = texture(u_cubemap, a_texCoords);
}

#end FRAGMENT
	)";

	Skybox::Skybox(const std::vector<Surface*>& skyboxTextures) {
		Load(skyboxTextures);
	}

	void Skybox::Load(const std::vector<Surface*>& skyboxTextures) {
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

		for (int i = 0; i < skyboxTextures.size(); i++) {
			auto tex = skyboxTextures.at(i);

			int mode = GL_RGB;
			if (tex->componentCount == 4)  {
				mode = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, tex->width, tex->height, 0, mode, GL_UNSIGNED_BYTE, tex->pixels);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		float skyboxVertices[] = {
			// positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(1, &m_va);
		glGenBuffers(1, &m_vb);
		glBindVertexArray(m_va);
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		m_shader = std::make_shared<Shader>(skyboxShader);
		m_shader->Bind();
		m_shader->SetInt("u_cubemap", 0);
	}

	void Skybox::Draw(Camera& camera) {
		glDepthMask(GL_FALSE);

		m_shader->Bind();
		auto view = glm::mat4(glm::mat3(camera.GetView()));
		m_shader->SetMat4("u_view", view);
		m_shader->SetMat4("u_projection", camera.projection);


		glBindVertexArray(m_va);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		glDepthMask(GL_TRUE);
	}
}
