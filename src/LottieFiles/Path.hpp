#pragma once

#include <vector>
#include "LinearAlgebra.hpp"

namespace LottieFiles {
	// path 2d class
	class Path2D {
	private:
		::std::vector<vec2> mPoints;
		vec2 mLastPoint;
	public:
		// begin path
		void beginPath();

		// close path
		void closePath();

		// move to
		void moveTo(float x, float y);

		// line to
		void lineTo(float x, float y);

		// bezier curve to
		void bezierCurveTo(
			float cp1x, float cp1y,
			float cp2x, float cp2y,
			float x, float y,
			unsigned int segs = 16);

		// arc cutve to
		void arcTo(float x, float y, float radiusX, float radiusY, float startAngle, float endAngle, size_t numSegs = 32);

		//get data pointer
		const float* getData() const;

		//get data size
		const size_t getDataSize() const;

		// get points count
		const size_t getPointsCount() const;
	};
}