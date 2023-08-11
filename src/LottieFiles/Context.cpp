#include "Context.hpp"

namespace LottieFiles {
	// constructor
	Context2D::Context2D() { 
		init();
	};

	// destructor
	Context2D::~Context2D() { 
		release();
	};

	// init
	void Context2D::init() {
		// create and setup fill position buffer
		GL_CHECK(glGenVertexArrays(1, &mVaoFill));
		GL_CHECK(glBindVertexArray(mVaoFill));
		GL_CHECK(glGenBuffers(1, &mVboFillPos));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVboFillPos));
		GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
		GL_CHECK(glEnableVertexAttribArray(0));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK(glBindVertexArray(0));
		// create and setup stroke position buffer
		GL_CHECK(glGenVertexArrays(1, &mVaoStroke));
		GL_CHECK(glBindVertexArray(mVaoStroke));
		GL_CHECK(glGenBuffers(1, &mVboStrokePos));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVboStrokePos));
		GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0));
		GL_CHECK(glEnableVertexAttribArray(0));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GL_CHECK(glBindVertexArray(0));
	};

	// release
	void Context2D::release() {
		// delete stroke position buffer
		GL_CHECK(glDeleteBuffers(1, &mVboStrokePos));
		GL_CHECK(glDeleteVertexArrays(1, &mVaoStroke));
		// delete fill position buffer
		GL_CHECK(glDeleteBuffers(1, &mVboFillPos));
		GL_CHECK(glDeleteVertexArrays(1, &mVaoStroke));
		// stroke opengl handles
		mVboStrokePos = 0;
		mVaoStroke = 0;
		// fill opengl handles
		mVboFillPos = 0;
		mVaoFill = 0;
	};

	// set viewport
	void Context2D::setViewport(int width, int height) {
		if (width == 0) width = 1;
		if (height == 0) height = 1;
		if ((mViewportWidth != width) || (mViewportHeight != height)) {
			// store viewport size
			mViewportWidth = width;
			mViewportHeight = height;

			// update fill buffer
			float fillBuff[] = {
				0.0f, 0.0f,
				0.0f, (float)mViewportHeight,
				(float)mViewportWidth, 0.0f,
				(float)mViewportWidth, (float)mViewportHeight };
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVboFillPos));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(fillBuff), fillBuff, GL_DYNAMIC_DRAW));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

			// update projection matrix
			mProjMat = mat4::ortho(0.0f, (float)mViewportWidth, 0.0f, (float)mViewportHeight, -1.0f, 1.0f);
			mBrushFill.setProjMat(mProjMat);
		}
	}

	// strike the path by defined brush
	void Context2D::strokePath(Path2D* path, Brush* brush) {
		// check for nulls
		if (!path || !brush) return;

		// update projection matrix
		brush->setProjMat(mProjMat);

		// blend parameters
		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// stroke path
		brush->use();
		// update path buffer
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVboStrokePos));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, path->getDataSize(), path->getData(), GL_DYNAMIC_DRAW));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		// draw path
		GL_CHECK(glBindVertexArray(mVaoStroke));
		GL_CHECK(glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)path->getPointsCount()));
		GL_CHECK(glBindVertexArray(0));
		brush->unuse();
		GL_CHECK(glDisable(GL_BLEND));
	}

	// strike the path by defined brush
	void Context2D::fillPath(Path2D* path, Brush* brush) {
		// check for nulls
		if (!path || !brush) return;

		// stencil write params
		GL_CHECK(glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE));
		GL_CHECK(glEnable(GL_STENCIL_TEST));
		GL_CHECK(glClearStencil(0x00));
		GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));
		GL_CHECK(glStencilMask(0xFF));
		GL_CHECK(glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT));
		GL_CHECK(glStencilFunc(GL_ALWAYS, 0xFF, 0xFF));
		// set color
		mBrushFill.use();
		// update path buffer
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVboStrokePos));
		GL_CHECK(glBufferData(GL_ARRAY_BUFFER, path->getDataSize(), path->getData(), GL_DYNAMIC_DRAW));
		GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
		// draw path
		GL_CHECK(glBindVertexArray(mVaoStroke));
		GL_CHECK(glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)path->getPointsCount()));
		GL_CHECK(glBindVertexArray(0));
		mBrushFill.unuse();
		GL_CHECK(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));

		// blend parameters
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// stencil test params
		GL_CHECK(glStencilFunc(GL_EQUAL, 0xFF, 0xFF));
		GL_CHECK(glStencilMask(0xFF));
		// set matrix
		brush->setProjMat(mat4::ortho(0, (float)mViewportWidth, 0, (float)mViewportHeight, -1, 1));
		brush->use();
		// fill path
		GL_CHECK(glBindVertexArray(mVaoFill));
		GL_CHECK(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
		GL_CHECK(glBindVertexArray(0));
		brush->unuse();
		GL_CHECK(glDisable(GL_BLEND));
		GL_CHECK(glDisable(GL_STENCIL_TEST));
	}
}