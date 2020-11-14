#include "Mesh.h"

#include <glad/glad.h>

#include "Utils/obj_loader.h"

#include "Logger.h"

#include <algorithm>
#include <initializer_list>

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

namespace Crimson {
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
		LoadFromData(vertices, indices);
	}

	Mesh::Mesh(const char* lua) {
		LoadFromLua(lua);
	}

	void Mesh::LoadFromData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
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

		CalculateAABB();
	}

	void Mesh::LoadFromLua(const char* lua) {
		std::vector<Vertex> verts;
		std::vector<unsigned int> indices;

		lua_State* L = luaL_newstate();

		int r = luaL_dostring(L, lua);
		if (r != LUA_OK) {
			CR_LOG_ERROR("Error loading mesh: %s", lua_tostring(L, -1));
			lua_close(L);
			return;
		}

		Vertex currentVertex;

		lua_getglobal(L, "positions");
		unsigned int positionsSize = lua_rawlen(L, -1);
		lua_pop(L, 1);

		lua_getglobal(L, "normals");
		unsigned int normalsSize = lua_rawlen(L, -1);
		lua_pop(L, 1);

		lua_getglobal(L, "texCoords");
		unsigned int texCoordsSize = lua_rawlen(L, -1);
		lua_pop(L, 1);

		unsigned int size = std::max({positionsSize, normalsSize, texCoordsSize});
		verts.resize(size/3);

		unsigned int texIt = 0;
		unsigned int posIt = 0;
		for (auto& vertex : verts) {
			// position
			lua_getglobal(L, "positions");
			lua_rawgeti(L, -1, posIt+1);
			vertex.position.x = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, posIt+2);
			vertex.position.y = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, posIt+3);
			vertex.position.z = lua_tonumber(L, -1);
			lua_pop(L, 2);

			// normal
			lua_getglobal(L, "normals");
			lua_rawgeti(L, -1, posIt+1);
			vertex.normal.x = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, posIt+2);
			vertex.normal.y = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, posIt+3);
			vertex.normal.z = lua_tonumber(L, -1);
			lua_pop(L, 2);

			// texCoords
			lua_getglobal(L, "texCoords");
			lua_rawgeti(L, -1, texIt+1);
			vertex.texCoords.x = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, texIt+2);
			vertex.texCoords.y = lua_tonumber(L, -1);
			lua_pop(L, 2);
			texIt+=2;
			posIt+=3;
		}
		lua_pop(L, 1);

		lua_getglobal(L, "indices");
		size = lua_rawlen(L, -1);
		for (unsigned int i = 0; i < size; i++) {
			lua_rawgeti(L, -1, i+1);
			indices.push_back(lua_tointeger(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);

		lua_close(L);

		LoadFromData(verts, indices);

	}

	AABB Mesh::CalculateAABB() {
		AABB result;

		result.maxCorner.x = result.minCorner.x = m_vertices[0].position.x;
		result.maxCorner.y = result.minCorner.y = m_vertices[0].position.y;
		result.maxCorner.z = result.minCorner.z = m_vertices[0].position.z;

		for (int i = 0; i < m_vertices.size(); i++) {
			if (m_vertices[i].position.x < result.minCorner.z) result.minCorner.z = m_vertices[i].position.x;
			if (m_vertices[i].position.x > result.maxCorner.x) result.maxCorner.x = m_vertices[i].position.x;
			if (m_vertices[i].position.y < result.minCorner.z) result.minCorner.z = m_vertices[i].position.y;
			if (m_vertices[i].position.y > result.maxCorner.y) result.maxCorner.y = m_vertices[i].position.y;
			if (m_vertices[i].position.z < result.minCorner.z) result.minCorner.z = m_vertices[i].position.z;
			if (m_vertices[i].position.z > result.maxCorner.z) result.maxCorner.z = m_vertices[i].position.z;
		}

		m_aabb = result;

		return result;
	}

	AABB Mesh::GetAABB() {
		return m_aabb;
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
