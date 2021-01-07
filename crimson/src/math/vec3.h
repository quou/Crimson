#pragma once

#include "core.h"

#include "vec2.h"

namespace Crimson {
	struct vec4;
	struct mat4;

	struct CR_API vec3 {
		float x, y, z;

		vec3();
		vec3(float scalar);
		vec3(float x, float y, float z);
		vec3(const vec2& other);
		vec3(float x, float y);
		vec3(const vec4& other);

		static vec3 up();
		static vec3 down();
		static vec3 left();
		static vec3 right();
		static vec3 zero();

		static vec3 xaxis();
		static vec3 yaxis();
		static vec3 zaxis();

		vec3& add(const vec3& other);
		vec3& subtract(const vec3& other);
		vec3& multiply(const vec3& other);
		vec3& divide(const vec3& other);

		vec3& add(float other);
		vec3& subtract(float other);
		vec3& multiply(float other);
		vec3& divide(float other);

		vec3 multiply(const mat4& transform) const;

		friend vec3 operator+(vec3 left, const vec3& right);
		friend vec3 operator-(vec3 left, const vec3& right);
		friend vec3 operator*(vec3 left, const vec3& right);
		friend vec3 operator/(vec3 left, const vec3& right);

		friend vec3 operator+(vec3 left, float right);
		friend vec3 operator-(vec3 left, float right);
		friend vec3 operator*(vec3 left, float right);
		friend vec3 operator/(vec3 left, float right);

		bool operator==(const vec3& other) const;
		bool operator!=(const vec3& other) const;

		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(const vec3& other);
		vec3& operator/=(const vec3& other);

		vec3& operator+=(float other);
		vec3& operator-=(float other);
		vec3& operator*=(float other);
		vec3& operator/=(float other);

		bool operator<(const vec3& other) const;
		bool operator<=(const vec3& other) const;
		bool operator>(const vec3& other) const;
		bool operator>=(const vec3& other) const;

		friend vec3 operator-(const vec3& vector);

		vec3 cross(const vec3& other) const;
		float dot(const vec3& other) const;

		float magnitude() const;
		vec3 normalised() const;
		float distance(const vec3& other) const;
	};
}
