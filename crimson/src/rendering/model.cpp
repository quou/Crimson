#include <vector>
#include <string>
#include <sstream>
#include <string.h>

#include "assets.h"
#include "model.h"
#include "logger.h"
#include "renderdata.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace Crimson {
	Model::Model() {

	}

	Model::~Model() {

	}

	void Model::Draw() {
		for (const ref<Mesh>& mesh : m_meshes) {
			mesh->Draw();
		}
	}


	void Model::AddMesh(const ref<Mesh>& mesh) {
		m_meshes.push_back(mesh);
	}
}
