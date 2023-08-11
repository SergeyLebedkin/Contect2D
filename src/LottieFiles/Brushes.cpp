#include <iostream>
#include <vector>
#include "Brushes.hpp"
#include "ShaderSources.hpp"

namespace LottieFiles {
	// create CreateShader
	bool Brush::CreateShader(const char* vertexSource, const char* fragmentSource)
	{
		// delete existing shader
		DeleteShader();

		// create and compile vertex shader
		mGLVertexShader = GL_CHECK(glCreateShader(GL_VERTEX_SHADER));
		GL_CHECK(glShaderSource(mGLVertexShader, 1, &vertexSource, nullptr));
		GL_CHECK(glCompileShader(mGLVertexShader));
		if (!ShaderStatusCheck(mGLVertexShader))
		{
			GL_CHECK(glDeleteProgram(mGLVertexShader));
			mGLVertexShader = 0;
			return false;
		}

		// create and compile fragment shader
		mGLFragmentShader = GL_CHECK(glCreateShader(GL_FRAGMENT_SHADER));
		GL_CHECK(glShaderSource(mGLFragmentShader, 1, &fragmentSource, nullptr));
		GL_CHECK(glCompileShader(mGLFragmentShader));
		if (!ShaderStatusCheck(mGLFragmentShader))
		{
			GL_CHECK(glDeleteShader(mGLFragmentShader));
			GL_CHECK(glDeleteShader(mGLVertexShader));
			mGLFragmentShader = 0;
			mGLVertexShader = 0;
			return false;
		}

		// attach shaders
		mGLProgram = GL_CHECK(glCreateProgram());
		GL_CHECK(glAttachShader(mGLProgram, mGLVertexShader));
		GL_CHECK(glAttachShader(mGLProgram, mGLFragmentShader));

		// link program
		GL_CHECK(glLinkProgram(mGLProgram));
		if (!ProgramStatusCheck(mGLProgram))
		{
			GL_CHECK(glDeleteProgram(mGLProgram));
			GL_CHECK(glDeleteProgram(mGLFragmentShader));
			GL_CHECK(glDeleteShader(mGLVertexShader));
			mGLProgram = 0;
			mGLFragmentShader = 0;
			mGLVertexShader = 0;
			return false;
		}

		return true;
	}

	void Brush::DeleteShader()
	{
		// delete shaders and program handle
		GL_CHECK(glDeleteProgram(mGLProgram));
		GL_CHECK(glDeleteShader(mGLFragmentShader));
		GL_CHECK(glDeleteShader(mGLVertexShader));
		mGLProgram = 0;
		mGLFragmentShader = 0;
		mGLVertexShader = 0;
	}

	// shader status check
	bool Brush::ShaderStatusCheck(GLuint shader)
	{
		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			// get max length of info log
			GLsizei maxLength = 0;
			GLsizei length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &length, &infoLog[0]);
			std::cerr << infoLog.data() << std::endl;

			// Provide the info log in whatever manner you deem best.
			// Exit with failure.
			return false;
		}
		return true;
	}

	// program status check
	bool Brush::ProgramStatusCheck(GLuint program)
	{
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			// get max length of info log
			GLint maxLength = 0;
			GLsizei length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &length, &infoLog[0]);
			std::cerr << infoLog.data() << std::endl;

			// Provide the info log in whatever manner you deem best.
			// Exit with failure.
			return false;
		}
		return true;
	}

	// use program
	void Brush::use() {
		GL_CHECK(glUseProgram(mGLProgram));
	}

	// unuse program
	void Brush::unuse() {
		GL_CHECK(glUseProgram(0));
	}

	// BrushSolid::BrushSolid
	BrushSolid::BrushSolid() {
		// create shader from source
		CreateShader(cVSShaderSource_Solid, cFSShaderSource_Solid);
		// get uniform locations
		uProjMatLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uProjMat"));
		uColorLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColor"));
	}

	// set projection matrix
	void BrushSolid::setProjMat(mat4 projMat) {
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniformMatrix4fv(uProjMatLoc, 1, GL_FALSE, (float*)&projMat));
		GL_CHECK(glUseProgram(0));
	}

	// set color
	void BrushSolid::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		// create color
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}

	// BrushRadial::BrushRadial
	BrushRadial::BrushRadial() {
		// create shader from source
		CreateShader(cVSShaderSource_Radial, cFSShaderSource_Radial);
		// get uniform locations
		uProjMatLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uProjMat"));
		uCenterLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uCenter"));
		uRadiusLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uRadius"));
		uColorCenterLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColorCenter"));
		uColorMiddleLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColorMiddle"));
		uColorOuterLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColorOuter"));
	}

	// set projection matrix
	void BrushRadial::setProjMat(mat4 projMat) {
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniformMatrix4fv(uProjMatLoc, 1, GL_FALSE, (float*)&projMat[0][0]));
		GL_CHECK(glUseProgram(0));
	}

	// set center
	void BrushRadial::setCenter(float x, float y) {
		// create color
		float center[] = { x, y };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform2fv(uCenterLoc, 1, center));
		GL_CHECK(glUseProgram(0));
	}

	// set radius
	void BrushRadial::setRadius(float radius) {
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform1f(uRadiusLoc, radius));
		GL_CHECK(glUseProgram(0));
	}

	// set color center
	void BrushRadial::setColorCenter(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorCenterLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}

	// set color middle
	void BrushRadial::setColorMiddle(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		// create color
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorMiddleLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}

	// set color outer
	void BrushRadial::setColorOuter(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		// create color
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorOuterLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}

	// BrushLinear::BrushLinear
	BrushLinear::BrushLinear() {
		// create shader from source
		CreateShader(cVSShaderSource_Linear, cFSShaderSource_Linear);
		// get uniform locations
		uProjMatLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uProjMat"));
		uLinePointsLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uLinePoints"));
		uDistanceLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uDistance"));
		uColorInnerLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColorInner"));
		uColorOuterLoc = GL_CHECK(glGetUniformLocation(mGLProgram, "uColorOuter"));
	}

	// set projection matrix
	void BrushLinear::setProjMat(mat4 projMat) {
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniformMatrix4fv(uProjMatLoc, 1, GL_FALSE, (float*)&projMat[0][0]));
		GL_CHECK(glUseProgram(0));
	}
	
	// set line points
	void BrushLinear::setLinePoints(float p1x, float p1y, float p2x, float p2y) {
		// create points
		float points[] = { p1x, p1y, p2x, p2y };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uLinePointsLoc, 1, points));
		GL_CHECK(glUseProgram(0));
	}

	// set distance
	void BrushLinear::setDistance(float distance) {
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform1f(uDistanceLoc, distance));
		GL_CHECK(glUseProgram(0));
	}

	// set inner color
	void BrushLinear::setColorInner(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		// create color
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorInnerLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}

	// set outer color
	void BrushLinear::setColorOuter(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		// create color
		float color[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
		GL_CHECK(glUseProgram(mGLProgram));
		GL_CHECK(glUniform4fv(uColorOuterLoc, 1, color));
		GL_CHECK(glUseProgram(0));
	}
}