#pragma once

namespace LottieFiles {
	// vector 2d
	struct vec2 {
		// values
		float x, y;

		// constructors
		vec2() : x(0.0f), y(0.0f) {}
		vec2(float x, float y) : x(x), y(y) {}

		// operators
		vec2 operator+(vec2 v) const { return vec2(this->x + v.x, this->y + v.y); }
		vec2 operator*(float v) const { return vec2(this->x * v, this->y * v); }
		float operator[](size_t index) const { return (&x)[index]; }
		float& operator[](size_t index) { return (&x)[index]; }
	};

	// vector 4d
	struct vec4 {
		// values
		float x, y, z, w;

		// constructors
		vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0) {}
		vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		float operator[](size_t index) const { return (&x)[index]; }
		float& operator[](size_t index) { return (&x)[index]; }
	};

	// matrix 4x4
	struct mat4 {
		vec4 raws[4];

		// operators
		vec4 operator[](size_t index) const { return raws[index]; }
		vec4& operator[](size_t index) { return raws[index]; }

		// ortho matrix
		static mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar) {
			mat4 res;
			res[0][0] = 2.0f / (right - left);
			res[1][1] = 2.0f / (top - bottom);
			res[2][2] = -2.0f / (zFar - zNear);
			res[3][0] = -(right + left) / (right - left);
			res[3][1] = -(top + bottom) / (top - bottom);
			res[3][2] = -(zFar + zNear) / (zFar - zNear);
			res[3][3] = 1.0f;
			return res;
		}
	};
}