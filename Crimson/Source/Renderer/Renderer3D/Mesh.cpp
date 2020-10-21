#include "Mesh.h"

#include <glad/glad.h>

namespace Crimson {
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
		m_vertices = vertices;
		m_indices = indices;

		glGenVertexArrays(1, &m_va);
		glGenBuffers(1, &m_vb);
		glGenBuffers(1, &m_ib);

		glBindVertexArray(m_va);
		glBindBuffer(GL_ARRAY_BUFFER, m_vb);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
	}

	void Mesh::Draw() {
		glBindVertexArray(m_va);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		glDeleteVertexArrays(1, &m_va);
		glDeleteBuffers(1, &m_vb);
		glDeleteBuffers(1, &m_ib);
	}
}
