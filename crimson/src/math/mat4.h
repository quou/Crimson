#pragma once

#include <string>

#include "core.h"
#include "vec4.h"
#include "vec3.h"

namespace Crimson {
	struct CR_API mat4 {
		union {
			// [row + col * 4]
			float elements[4 * 4];
			vec4 rows[4];
		};

		mat4();
		mat4(float diagonal);
		mat4(float* elements);
		mat4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3);

		static mat4 identity();

		mat4& multiply(const mat4& other);
		friend mat4 operator*(mat4 left, const mat4& right);
		mat4& operator*=(const mat4& other);

		vec3 multiply(const vec3& other) const;
		friend vec3 operator*(const mat4& left, const vec3& right);

		vec4 multiply(const vec4& other) const;
		friend vec4 operator*(const mat4& left, const vec4& right);

		mat4& invert();

		vec4 GetColumn(int index) const;
		void SetColumn(unsigned int index, const vec4& column);
		inline vec3 GetPosition() const { return vec3(GetColumn(3)); }
		inline void SetPosition(const vec3& position) { 
			SetColumn(3, vec4(position, 1.0f)); 
		}

		static mat4 ortho(float left, float right, float bottom, 
				float top, float near, float far);
		static mat4 persp(float fov, float aspectRatio, float near, float far);
		static mat4 lookat(const vec3& camera, const vec3& object, const vec3& up);

		static mat4 translate(const vec3& translation);
		static mat4 rotate(float angle, const vec3& axis);
		static mat4 scale(const vec3& scale);
		static mat4 invert(const mat4& matrix);

		static mat4 transpose(const mat4& matrix);

		std::string str();
	};
}
