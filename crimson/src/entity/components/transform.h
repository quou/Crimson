#pragma once

#include "math/mat4.h"
#include "entity/entity.h"

namespace Crimson {
	class TransformComponent : public Component {
	private:
		mat4 m_transform;
	public:
		TransformComponent();

		void Reset();

		void Translate(const vec3& t);
		void Rotate(float angle, const vec3& r);
		void Scale(const vec3& s);

		inline const mat4& GetMatrix() const { return m_transform; }
	};
}