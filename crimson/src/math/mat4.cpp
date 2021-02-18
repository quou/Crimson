#include <string.h>
#include <stdint.h>
#include <math.h>
#include <sstream>

#include "mat4.h"
#include "maths.h"

namespace Crimson {
	mat4::mat4() {
		memset(elements, 0, 4 * 4 * sizeof(float));
	}

	mat4::mat4(float diagonal) {
		memset(elements, 0, 4 * 4 * sizeof(float));
		elements[0 + 0 * 4] = diagonal;
		elements[1 + 1 * 4] = diagonal;
		elements[2 + 2 * 4] = diagonal;
		elements[3 + 3 * 4] = diagonal;
	}

	mat4::mat4(float* elements) {
		memcpy(this->elements, elements, 4 * 4 * sizeof(float));
	}

	mat4::mat4(const vec4& row0, const vec4& row1, const vec4& row2, const vec4& row3) {
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
		rows[3] = row3;
	}
	
	mat4 mat4::identity() {
		return mat4(1.0f);
	}

	mat4& mat4::multiply(const mat4& other) {
		float data[16];
		for (int32_t row = 0; row < 4; row++)
		{
			for (int32_t col = 0; col < 4; col++)
			{
				float sum = 0.0f;
				for (int32_t e = 0; e < 4; e++)
				{
					sum += elements[e + row * 4] * 
						other.elements[col + e * 4];
				}
				data[col + row * 4] = sum;
			}
		}
		memcpy(elements, data, 4 * 4 * sizeof(float));
		return *this;
	}

	vec3 mat4::multiply(const vec3& other) const
	{
		return other.multiply(*this);
	}

	vec4 mat4::multiply(const vec4& other) const
	{
		return other.multiply(*this);
	}

	mat4 operator*(mat4 left, const mat4& right) {
		return left.multiply(right);
	}

	mat4& mat4::operator*=(const mat4& other) {
		return multiply(other);
	}

	vec3 operator*(const mat4& left, const vec3& right) {
		return left.multiply(right);
	}

	vec4 operator*(const mat4& left, const vec4& right) {
		return left.multiply(right);
	}

	mat4& mat4::invert() {
		float temp[16];

		temp[0] = elements[5] * elements[10] * elements[15] -
			elements[5] * elements[11] * elements[14] -
			elements[9] * elements[6] * elements[15] +
			elements[9] * elements[7] * elements[14] +
			elements[13] * elements[6] * elements[11] -
			elements[13] * elements[7] * elements[10];

		temp[4] = -elements[4] * elements[10] * elements[15] +
			elements[4] * elements[11] * elements[14] +
			elements[8] * elements[6] * elements[15] -
			elements[8] * elements[7] * elements[14] -
			elements[12] * elements[6] * elements[11] +
			elements[12] * elements[7] * elements[10];

		temp[8] = elements[4] * elements[9] * elements[15] -
			elements[4] * elements[11] * elements[13] -
			elements[8] * elements[5] * elements[15] +
			elements[8] * elements[7] * elements[13] +
			elements[12] * elements[5] * elements[11] -
			elements[12] * elements[7] * elements[9];

		temp[12] = -elements[4] * elements[9] * elements[14] +
			elements[4] * elements[10] * elements[13] +
			elements[8] * elements[5] * elements[14] -
			elements[8] * elements[6] * elements[13] -
			elements[12] * elements[5] * elements[10] +
			elements[12] * elements[6] * elements[9];

		temp[1] = -elements[1] * elements[10] * elements[15] +
			elements[1] * elements[11] * elements[14] +
			elements[9] * elements[2] * elements[15] -
			elements[9] * elements[3] * elements[14] -
			elements[13] * elements[2] * elements[11] +
			elements[13] * elements[3] * elements[10];

		temp[5] = elements[0] * elements[10] * elements[15] -
			elements[0] * elements[11] * elements[14] -
			elements[8] * elements[2] * elements[15] +
			elements[8] * elements[3] * elements[14] +
			elements[12] * elements[2] * elements[11] -
			elements[12] * elements[3] * elements[10];

		temp[9] = -elements[0] * elements[9] * elements[15] +
			elements[0] * elements[11] * elements[13] +
			elements[8] * elements[1] * elements[15] -
			elements[8] * elements[3] * elements[13] -
			elements[12] * elements[1] * elements[11] +
			elements[12] * elements[3] * elements[9];

		temp[13] = elements[0] * elements[9] * elements[14] -
			elements[0] * elements[10] * elements[13] -
			elements[8] * elements[1] * elements[14] +
			elements[8] * elements[2] * elements[13] +
			elements[12] * elements[1] * elements[10] -
			elements[12] * elements[2] * elements[9];

		temp[2] = elements[1] * elements[6] * elements[15] -
			elements[1] * elements[7] * elements[14] -
			elements[5] * elements[2] * elements[15] +
			elements[5] * elements[3] * elements[14] +
			elements[13] * elements[2] * elements[7] -
			elements[13] * elements[3] * elements[6];

		temp[6] = -elements[0] * elements[6] * elements[15] +
			elements[0] * elements[7] * elements[14] +
			elements[4] * elements[2] * elements[15] -
			elements[4] * elements[3] * elements[14] -
			elements[12] * elements[2] * elements[7] +
			elements[12] * elements[3] * elements[6];

		temp[10] = elements[0] * elements[5] * elements[15] -
			elements[0] * elements[7] * elements[13] -
			elements[4] * elements[1] * elements[15] +
			elements[4] * elements[3] * elements[13] +
			elements[12] * elements[1] * elements[7] -
			elements[12] * elements[3] * elements[5];

		temp[14] = -elements[0] * elements[5] * elements[14] +
			elements[0] * elements[6] * elements[13] +
			elements[4] * elements[1] * elements[14] -
			elements[4] * elements[2] * elements[13] -
			elements[12] * elements[1] * elements[6] +
			elements[12] * elements[2] * elements[5];

		temp[3] = -elements[1] * elements[6] * elements[11] +
			elements[1] * elements[7] * elements[10] +
			elements[5] * elements[2] * elements[11] -
			elements[5] * elements[3] * elements[10] -
			elements[9] * elements[2] * elements[7] +
			elements[9] * elements[3] * elements[6];

		temp[7] = elements[0] * elements[6] * elements[11] -
			elements[0] * elements[7] * elements[10] -
			elements[4] * elements[2] * elements[11] +
			elements[4] * elements[3] * elements[10] +
			elements[8] * elements[2] * elements[7] -
			elements[8] * elements[3] * elements[6];

		temp[11] = -elements[0] * elements[5] * elements[11] +
			elements[0] * elements[7] * elements[9] +
			elements[4] * elements[1] * elements[11] -
			elements[4] * elements[3] * elements[9] -
			elements[8] * elements[1] * elements[7] +
			elements[8] * elements[3] * elements[5];

		temp[15] = elements[0] * elements[5] * elements[10] -
			elements[0] * elements[6] * elements[9] -
			elements[4] * elements[1] * elements[10] +
			elements[4] * elements[2] * elements[9] +
			elements[8] * elements[1] * elements[6] -
			elements[8] * elements[2] * elements[5];

		float determinant = elements[0] * temp[0] + elements[1] * 
			temp[4] + elements[2] * temp[8] + elements[3] * temp[12];
		determinant = 1.0f / determinant;

		for (int32_t i = 0; i < 4 * 4; i++)
			elements[i] = temp[i] * determinant;

		return *this;
	}

	vec4 mat4::GetColumn(int index) const {
		return vec4(elements[index + 0 * 4], 
				elements[index + 1 * 4], 
				elements[index + 2 * 4], 
				elements[index + 3 * 4]);
	}

	void mat4::SetColumn(unsigned int index, const vec4& column) {
		elements[index + 0 * 4] = column.x;
		elements[index + 1 * 4] = column.y;
		elements[index + 2 * 4] = column.z;
		elements[index + 3 * 4] = column.w;
	}

	mat4 mat4::ortho(float left, float right, float bottom, 
			float top, float near, float far) {
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = 2.0f / (right - left);

		result.elements[1 + 1 * 4] = 2.0f / (top - bottom);

		result.elements[2 + 2 * 4] = 2.0f / (near - far);

		result.elements[3 + 0 * 4] = (left + right) / (left - right);
		result.elements[3 + 1 * 4] = (bottom + top) / (bottom - top);
		result.elements[3 + 2 * 4] = (far + near) / (far - near);

		return result;
	}

	mat4 mat4::persp(float fov, float aspectRatio, float near, float far) {
		mat4 result(1.0f);

		float q = 1.0f / tan(Radians(0.5f * fov));
		float a = q / aspectRatio;

		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.elements[0 + 0 * 4] = a;
		result.elements[1 + 1 * 4] = q;
		result.elements[2 + 2 * 4] = b;
		result.elements[2 + 3 * 4] = -1.0f;
		result.elements[3 + 2 * 4] = c;

		return result;
	}

	mat4 mat4::lookat(const vec3& camera, const vec3& object, const vec3& up) {
		mat4 result(1.0f);
		vec3 f = (object - camera).normalised();
		vec3 u = up.normalised();
		vec3 s = f.cross(u).normalised();
		u = s.cross(f);

		result.elements[0 + 0 * 4] = s.x;
		result.elements[1 + 0 * 4] = s.y;
		result.elements[2 + 0 * 4] = s.z;
		result.elements[0 + 1 * 4] = u.x;
		result.elements[1 + 1 * 4] = u.y;
		result.elements[2 + 1 * 4] = u.z;
		result.elements[0 + 2 * 4] =-f.x;
		result.elements[1 + 2 * 4] =-f.y;
		result.elements[2 + 2 * 4] =-f.z;

		result.elements[3 + 0 * 4] =-s.dot(camera);
		result.elements[3 + 1 * 4] =-u.dot(camera);
		result.elements[3 + 2 * 4] = f.dot(camera);

		return result;
	}

	mat4 mat4::translate(const vec3& translation) {
		mat4 result(1.0f);

		result.elements[3 + 0 * 4] = translation.x;
		result.elements[3 + 1 * 4] = translation.y;
		result.elements[3 + 2 * 4] = translation.z;

		return result;
	}

	mat4 mat4::rotate(float angle, const vec3& axis) {
		mat4 result(1.0f);

		float r = Radians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;
		
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.elements[0 + 0 * 4] = x * x * omc + c;
		result.elements[0 + 1 * 4] = y * x * omc + z * s;
		result.elements[0 + 2 * 4] = x * z * omc - y * s;

		result.elements[1 + 0 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * y * omc + c;
		result.elements[1 + 2 * 4] = y * z * omc + x * s;

		result.elements[2 + 0 * 4] = x * z * omc + y * s;
		result.elements[2 + 1 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * z * omc + c;
		
		return result;
	}
	
	mat4 mat4::scale(const vec3& scale) {
		mat4 result(1.0f);

		result.elements[0 + 0 * 4] = scale.x;
		result.elements[1 + 1 * 4] = scale.y;
		result.elements[2 + 2 * 4] = scale.z;

		return result;
	}

	mat4 mat4::invert(const mat4& matrix) {
		mat4 result = matrix;
		return result.invert();
	}

	mat4 mat4::transpose(const mat4& matrix) {
		return mat4(
			vec4(matrix.rows[0].x, matrix.rows[1].x, matrix.rows[2].x, matrix.rows[3].x),
			vec4(matrix.rows[0].y, matrix.rows[1].y, matrix.rows[2].y, matrix.rows[3].y),
			vec4(matrix.rows[0].z, matrix.rows[1].z, matrix.rows[2].z, matrix.rows[3].z),
			vec4(matrix.rows[0].w, matrix.rows[1].w, matrix.rows[2].w, matrix.rows[3].w)
			);
	}

	std::string mat4::str() {
		std::stringstream result;
		result << "mat4: (" << rows[0].x << ", " << rows[1].x << ", " << rows[2].x << ", " << rows[3].x << "), ";
		result << "(" << rows[0].y << ", " << rows[1].y << ", " << rows[2].y << ", " << rows[3].y << "), ";
		result << "(" << rows[0].z << ", " << rows[1].z << ", " << rows[2].z << ", " << rows[3].z << "), ";
		result << "(" << rows[0].w << ", " << rows[1].w << ", " << rows[2].w << ", " << rows[3].w << ")";
		return result.str();
	}
}
