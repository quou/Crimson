#include "vec4.h"

#include "mat4.h"

namespace Crimson {
	vec4::vec4(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
	vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
	vec4::vec4(const vec3& xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

	vec4& vec4::add(const vec4& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;

		return *this;
	}

	vec4& vec4::subtract(const vec4& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;

		return *this;
	}

	vec4& vec4::multiply(const vec4& other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;

		return *this;
	}

	vec4& vec4::divide(const vec4& other) {
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;

		return *this;
	}

	vec4 vec4::multiply(const mat4& transform) const {
		return vec4(
			transform.rows[0].x * x + transform.rows[0].y * 
			y + transform.rows[0].z * z + transform.rows[0].w * w,
			transform.rows[1].x * x + transform.rows[1].y * 
			y + transform.rows[1].z * z + transform.rows[1].w * w,
			transform.rows[2].x * x + transform.rows[2].y * 
			y + transform.rows[2].z * z + transform.rows[2].w * w,
			transform.rows[3].x * x + transform.rows[3].y * 
			y + transform.rows[3].z * z + transform.rows[3].w * w
			);
	}

	vec4 operator+(vec4 left, const vec4& right) {
		return left.add(right);
	}

	vec4 operator-(vec4 left, const vec4& right) {
		return left.subtract(right);
	}

	vec4 operator*(vec4 left, const vec4& right) {
		return left.multiply(right);
	}

	vec4 operator/(vec4 left, const vec4& right) {
		return left.divide(right);
	}

	vec4& vec4::operator+=(const vec4& other) {
		return add(other);
	}

	vec4& vec4::operator-=(const vec4& other) {
		return subtract(other);
	}

	vec4& vec4::operator*=(const vec4& other) {
		return multiply(other);
	}

	vec4& vec4::operator/=(const vec4& other) {
		return divide(other);
	}

	bool vec4::operator==(const vec4& other) {
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool vec4::operator!=(const vec4& other) {
		return !(*this == other);
	}

	float vec4::dot(const vec4& other) {
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
}
