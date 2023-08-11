#include "Path.hpp"
#include <cmath>

namespace LottieFiles {
	// begin path
	void Path2D::beginPath() {
		mPoints.clear();
		mLastPoint = vec2(0.0f, 0.0f);
	}

	// close path
	void Path2D::closePath() {
		if (mPoints.size() > 2)
			lineTo(mPoints[0].x, mPoints[0].y);
	}

	// move to
	void Path2D::moveTo(float x, float y) {
		mLastPoint.x = x;
		mLastPoint.y = y;
	}

	// line to
	void Path2D::lineTo(float x, float y) {
		if (mPoints.empty())
			mPoints.push_back(mLastPoint);
		mLastPoint = vec2(x, y);
		mPoints.push_back(mLastPoint);
	}

	// bezier curve to
	void Path2D::bezierCurveTo(
		float cp1x, float cp1y,
		float cp2x, float cp2y,
		float x, float y,
		unsigned int segs) {
		vec2 p0 = mLastPoint;
		vec2 p1 = vec2(cp1x, cp1y);
		vec2 p2 = vec2(cp2x, cp2y);
		vec2 p3 = vec2(x, y);
		for (unsigned int i = 1; i <= segs; i++) {
			float t = i / (float)segs;
			vec2 pr =
				p0 * 1 * (1.0f - t) * (1.0f - t) * (1.0f - t) +
				p1 * 3 * (1.0f - t) * (1.0f - t) * t +
				p2 * 3 * (1.0f - t) * t * t +
				p3 * 1 * t * t * t;
			lineTo(pr.x, pr.y);
		}
	};

	// arc cutve to
	void Path2D::arcTo(float x, float y, float radiusX, float radiusY, float startAngle, float endAngle, size_t numSegs) {
		for (size_t i = 0; i <= numSegs; i++) {
			float angle = ((endAngle - startAngle) * i / numSegs + startAngle) * 0.0174532925199433f;
			float pX = std::cos(angle) * radiusX + x;
			float pY = std::sin(angle) * radiusY + y;
			//lineTo(pX, pY);
			mPoints.push_back(vec2(pX, pY));
			mLastPoint = vec2(pX, pY);
		}
	}

	//get data pointer
	const float* Path2D::getData() const {
		return (float*)mPoints.data();
	}

	//get data size
	const size_t Path2D::getDataSize() const {
		return mPoints.size() * 2 * 4;
	}

	// get points count
	const size_t Path2D::getPointsCount() const {
		return mPoints.size();
	}
}