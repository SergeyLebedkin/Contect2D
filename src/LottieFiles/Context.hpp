#pragma once

#include "Path.hpp"
#include "Brushes.hpp"

namespace LottieFiles {
	// context 2d
	class Context2D {
	private:
		// offscreen fill brush
		BrushSolid mBrushFill;
	private:
		// viewport
		int mViewportWidth = 0;
		int mViewportHeight = 0;

		// metrices
		mat4 mProjMat;

		// stroke opengl handles
		GLuint mVboStrokePos = 0;
		GLuint mVaoStroke = 0;

		// fill opengl handles
		GLuint mVboFillPos = 0;
		GLuint mVaoFill = 0;
	private:
		// init
		void init();
		// release
		void release();
	public:
		// construcor
		Context2D();
		~Context2D();

		// set viewport
		void setViewport(int width, int height);

		// stroke the path by defined brush
		void strokePath(Path2D* path, Brush* brush);

		// fill the path by defined brush
		void fillPath(Path2D* path, Brush* brush);
	};
}