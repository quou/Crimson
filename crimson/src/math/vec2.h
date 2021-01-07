#pragma once

namespace Crimson {
	struct vec2 {
		float x;
		float y;

		vec2() : x(0.0f), y(0.0f) {}
		vec2(float xy) : x(xy), y(xy) {}
		vec2(float x, float y) : x(x), y(y) {}

		vec2& add(const vec2& other);
		vec2& subtract(const vec2& other);
		vec2& multiply(const vec2& other);
		vec2& divide(const vec2& other);

		vec2& add(float value);
		vec2& subtract(float value);
		vec2& multiply(float value);
		vec2& divide(float value);

		friend vec2 operator+(vec2 left, const vec2& right);
		friend vec2 operator-(vec2 left, const vec2& right);
		friend vec2 operator*(vec2 left, const vec2& right);
		friend vec2 operator/(vec2 left, const vec2& right);

		friend vec2 operator+(vec2 left, float value);
		friend vec2 operator-(vec2 left, float value);
		friend vec2 operator*(vec2 left, float value);
		friend vec2 operator/(vec2 left, float value);

		bool operator==(const vec2& other) const;
		bool operator!=(const vec2& other) const;

		vec2& operator+=(const vec2& other);
		vec2& operator-=(const vec2& other);
		vec2& operator*=(const vec2& other);
		vec2& operator/=(const vec2& other);

		vec2& operator+=(float value);
		vec2& operator-=(float value);
		vec2& operator*=(float value);
		vec2& operator/=(float value);

		bool operator<(const vec2& other) const;
		bool operator<=(const vec2& other) const;
		bool operator>(const vec2& other) const;
		bool operator>=(const vec2& other) const;

		float magnitude() const;
		vec2 normalised() const;
		float distance(const vec2& other) const;
		float dot(const vec2& other) const;
	};
}
