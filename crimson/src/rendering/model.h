#pragma once

#include "core.h"
#include "mesh.h"
#include "memory.h"
#include "camera.h"
#include "entity/scene.h"
#include "math/mat4.h"

namespace Crimson {
	class CR_API Model {
	private:
		std::vector<ref<Mesh>> m_meshes;

		mat4 m_transform;

		std::string m_path;
		bool m_fromFile;
	public:
		Model();

		/* Initiate the meshes from a .fbx file */
		Model(const char* path);
		virtual ~Model();

		void AddMesh(const ref<Mesh>& mesh);
	
		inline void SetTransform(const mat4& t) { m_transform = t; }
		
		/* Draw the scene using a predefined shader */
		void Draw(Shader* shader);

		/* Draw the model as part of a complete scene, including lighting */
		void DrawLitScene(const Camera& camera, Scene* scene);

		inline std::vector<ref<Mesh>>& GetMeshList() { return m_meshes; }
		inline void ClearMeshes() { m_fromFile = false; m_meshes.clear(); }

		/* For file management */
		inline const std::string& GetPath() const { return m_path; }
		inline bool IsFromFile() const { return m_fromFile; }

		/* Returns the first mesh in the mesh array,
		 * adds and returns a sphere mesh if none exist */
		ref<Mesh>& GetFirstMesh();
	};
}
