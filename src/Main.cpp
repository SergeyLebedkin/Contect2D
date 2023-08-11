#include <iostream>
#include <GLFW/glfw3.h>
#include "LottieFiles/Context.hpp"

LottieFiles::Context2D* ctx = nullptr;
LottieFiles::BrushSolid* brushSolid = nullptr;
LottieFiles::BrushRadial* brushRadial = nullptr;
LottieFiles::BrushLinear* brushLinear = nullptr;
LottieFiles::Path2D* pathBezierPie = nullptr;
LottieFiles::Path2D* pathRadialPie = nullptr;
LottieFiles::Path2D* pathEllipse = nullptr;

// init
void init() {
	// create context
	ctx = new LottieFiles::Context2D();

	// create radial brush
	brushRadial = new LottieFiles::BrushRadial();
	brushRadial->setCenter(200.0f, 200.0f);
	brushRadial->setRadius(150.0f);
	brushRadial->setColorCenter(255, 0, 0, 255);
	brushRadial->setColorMiddle(255, 255, 0, 255);
	brushRadial->setColorOuter(255, 255, 255, 255);

	// create solid brush
	brushSolid = new LottieFiles::BrushSolid();
	brushSolid->setColor(255, 0, 0, 255);

	// create linear brush
	brushLinear = new LottieFiles::BrushLinear();
	brushLinear->setDistance(400.0f);
	brushLinear->setLinePoints(0.0f, 0.0f, 800.0f, 800.0f);
	brushLinear->setColorInner(0, 0, 255, 255);
	brushLinear->setColorOuter(255, 255, 0, 255);

	// create bezier pie path
	pathBezierPie = new LottieFiles::Path2D();
	pathBezierPie->beginPath();
	pathBezierPie->moveTo(400, 100);
	pathBezierPie->bezierCurveTo(566, 100, 700, 234, 700, 400);
	pathBezierPie->bezierCurveTo(700, 566, 566, 700, 400, 700);
	pathBezierPie->bezierCurveTo(234, 700, 100, 566, 100, 400);
	pathBezierPie->lineTo(400, 400);
	pathBezierPie->lineTo(400, 100);

	// create radial pie path
	pathRadialPie = new LottieFiles::Path2D();
	pathRadialPie->beginPath();
	pathRadialPie->moveTo(200, 200);
	pathRadialPie->arcTo(200, 200, 150, 150, -180, 90);
	pathRadialPie->lineTo(200, 200);
	pathRadialPie->closePath();

	// create ellipse path
	pathEllipse = new LottieFiles::Path2D();
	pathEllipse->beginPath();
	pathEllipse->arcTo(70, 600, 50, 100, 0, 360);
}

// draw
void draw(int width, int height) {
	GL_CHECK(glViewport(0, 0, width, height));
	GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GL_CHECK(glClearDepth(1.0));
	GL_CHECK(glDisable(GL_CULL_FACE));
	GL_CHECK(glDisable(GL_DEPTH_TEST));
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	
	// set viewport for context
	ctx->setViewport(width, height);

	// draw radial pie
	brushRadial->setCenter(200.0f, 200.0f);
	brushRadial->setRadius(150.0f);
	brushRadial->setColorCenter(255, 0, 0, 255);
	brushRadial->setColorMiddle(255, 255, 0, 255);
	brushRadial->setColorOuter(255, 255, 255, 255);
	brushSolid->setColor(255, 128, 0, 255);
	ctx->fillPath(pathRadialPie, brushRadial);

	// draw bezier pie
	brushLinear->setDistance(400.0f);
	brushLinear->setLinePoints(0.0f, 0.0f, 800.0f, 800.0f);
	brushLinear->setColorInner(0, 0, 255, 255);
	brushLinear->setColorOuter(255, 255, 0, 255);
	ctx->fillPath(pathBezierPie, brushLinear);
	ctx->strokePath(pathBezierPie, brushSolid);

	// draw ellipse pie
	ctx->fillPath(pathEllipse, brushSolid);
	ctx->strokePath(pathEllipse, brushSolid);
}

// release
void release() {
	// delete assets
	delete brushSolid;
	delete brushRadial;
	delete brushLinear;
	delete pathEllipse;
	delete pathBezierPie;
	delete pathRadialPie;

	// delete context
	delete ctx;
}

// main
int main(void) {
	// set error callback
	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << description << std::endl;
	});

	// init glfw
	if (!glfwInit()) return -1;

	// set opengl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// create a windowed mode window and its opengl context
	GLFWwindow* window = glfwCreateWindow(800, 800, "LF Home Test", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// make the window's context current
	glfwMakeContextCurrent(window);

	// init opengl extentions
	InitOpenGL4();

	// init context objects
	init();

	// loop until the user closes the window
	while (!glfwWindowShouldClose(window)) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// draw stuff
		draw(width, height);

		// swap front and back buffers
		glfwSwapBuffers(window);

		// poll for and process events
		glfwPollEvents();
	}
	
	// release context objects
	release();

	// terminate glfw
	glfwTerminate();

	return 0;
}