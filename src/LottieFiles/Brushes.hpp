#pragma once
#include "OpenGL4.hpp"
#include "LinearAlgebra.hpp"

namespace LottieFiles {
	// created SL-230809
	class Brush
	{
	protected:
		// OpenGL handles and settings
		GLuint mGLVertexShader = 0;
		GLuint mGLFragmentShader = 0;
		GLuint mGLProgram = 0;
	protected:
		// constructor and destructor
		Brush() {}
		virtual ~Brush() {
			// delete shader
			DeleteShader();
		}

		// create shader
		bool CreateShader(const char* vertexSource, const char* fragmentSource);
		void DeleteShader();

		// status check functions
		static bool ShaderStatusCheck(GLuint shader);
		static bool ProgramStatusCheck(GLuint program);
	public:
		// set projection matrix
		virtual void setProjMat(mat4 projMat) = 0;

		// use
		void use();
		void unuse();
	};

	// created SL-230809
	class BrushSolid : public Brush
	{
	private:
		// uniform locations
		GLint uProjMatLoc = -1;
		GLint uColorLoc = -1;
	public:
		// constructor
		BrushSolid();

		// set params
		void setProjMat(mat4 projMat) override;
		void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};

	// created SL-230809
	class BrushRadial : public Brush
	{
	private:
		// uniform locations
		GLint uProjMatLoc = -1;
		GLint uCenterLoc = -1;
		GLint uRadiusLoc = -1;
		GLint uColorCenterLoc = -1;
		GLint uColorMiddleLoc = -1;
		GLint uColorOuterLoc = -1;
	public:
		// constructor
		BrushRadial();

		// set params
		void setProjMat(mat4 projMat);
		void setCenter(float x, float y);
		void setRadius(float radius);
		void setColorCenter(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void setColorMiddle(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void setColorOuter(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};

	// created SL-230809
	class BrushLinear : public Brush
	{
	private:
		// uniform locations
		GLint uProjMatLoc = -1;
		GLint uLinePointsLoc = -1;
		GLint uDistanceLoc = -1;
		GLint uColorInnerLoc = -1;
		GLint uColorOuterLoc = -1;
	public:
		// constructor
		BrushLinear();

		// set params
		void setProjMat(mat4 projMat);
		void setLinePoints(float p1x, float p1y, float p2x, float p2y);
		void setDistance(float distance);
		void setColorInner(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void setColorOuter(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	};
}