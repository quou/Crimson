#pragma once

#include <vector>

#include "core.h"
#include "renderdata.h"
#include "memory.h"
#include "material.h"

namespace Crimson {
	class CR_API Mesh {
	public:
		/* Tells us where the mesh came from */
		enum InstanceType {
			CUSTOM, /* Created by the user */
			INSTANCE, /* A mesh factory built-in mesh */
		};

		/* Only used for mesh factory meshes */
		enum Type {
			CUBE,
			SPHERE
		};
	private:
		unsigned int m_va, m_vb, m_ib;

		unsigned int m_indexCount;

		ref<Material> m_material;

		InstanceType m_instanceType;
		Type m_factoryType;

		friend class Model;
		friend class MeshFactory;
	public:
		Mesh(const std::vector<Vertex>& vertices, 
				const std::vector<unsigned int>& indices, const ref<Material>& material);

		inline void UseMaterial(const ref<Material>& material) { m_material = material; }
		
		virtual ~Mesh();

		void LoadFromVertexData(const std::vector<Vertex>& vertices,
				const std::vector<unsigned int>& indices);

		void Draw();
		void DrawNoMaterial();

		inline const ref<Material>& GetMaterial() const { return m_material; }
		inline InstanceType GetInstanceType() const { return m_instanceType; }
		inline Type GetFactoryType() const { return m_factoryType; }
	};
}
