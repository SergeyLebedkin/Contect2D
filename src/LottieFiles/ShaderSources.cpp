#include "ShaderSources.hpp"
#include <string>

namespace LottieFiles {

	// vertex shader for texture solid
	const char* cVSShaderSource_Solid = R"(

#version 410 core

// attributes
layout (location = 0) in vec2 aPosition;

// uniforms
uniform mat4 uProjMat;

// main
void main()
{
	gl_Position = uProjMat * vec4(aPosition.xy, 0.0, 1.0);
}

)";

	// fragment shader for texture processing
	const char* cFSShaderSource_Solid = R"(

#version 410 core

// uniforms
uniform vec4 uColor;

// outputs
layout (location = 0) out vec4 fragColor;

void main()
{
	fragColor = uColor;
}

)";

	// vertex shader for radial brush
	const char* cVSShaderSource_Radial = R"(

#version 410 core

// attributes
layout (location = 0) in vec2 aPosition;

// uniforms
uniform mat4 uProjMat;

// outputs
out vec2 vScreenCoord;

// main
void main()
{
	vScreenCoord = aPosition;
	gl_Position = uProjMat * vec4(aPosition, 0.0, 1.0);
}

)";

	// fragment shader for radial brush
	const char* cFSShaderSource_Radial = R"(

#version 410 core

// inputs
in vec2 vScreenCoord;

// uniforms
uniform vec2 uCenter;
uniform float uRadius;
uniform vec4 uColorCenter;
uniform vec4 uColorMiddle;
uniform vec4 uColorOuter;

// outputs
layout (location = 0) out vec4 fragColor;

void main()
{
	// get distances
	float dist = distance(vScreenCoord, uCenter);
	float halfRadius = uRadius / 2;

	// get color
	vec4 color = uColorOuter;
	if (dist < halfRadius)
		color = mix(uColorCenter, uColorMiddle, dist / halfRadius);
	else if (dist < uRadius)
		color = mix(uColorMiddle, uColorOuter, (dist - halfRadius) / halfRadius);

	// write result
	fragColor = color;
}

)";

	// vertex shader for radial brush
	const char* cVSShaderSource_Linear = R"(

#version 410 core

// attributes
layout (location = 0) in vec2 aPosition;

// uniforms
uniform mat4 uProjMat;

// outputs
out vec2 vScreenCoord;

// main
void main()
{
	vScreenCoord = aPosition;
	gl_Position = uProjMat * vec4(aPosition, 0.0, 1.0);
}

)";

	// fragment shader for radial brush
	const char* cFSShaderSource_Linear = R"(

#version 410 core

// inputs
in vec2 vScreenCoord;

// uniforms
uniform vec4 uLinePoints;
uniform float uDistance;
uniform vec4 uColorInner;
uniform vec4 uColorOuter;

// outputs
layout (location = 0) out vec4 fragColor;

void main()
{
	// get distances
	vec2 p0 = vScreenCoord.xy;
	vec2 p1 = uLinePoints.xy;
	vec2 p2 = uLinePoints.zw;

	float dist = 
		abs((p2.x - p1.x)*(p1.y - p0.y) - (p1.x - p0.x)*(p2.y - p1.y)) /
		sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));

	// write result
	fragColor = mix(uColorInner, uColorOuter, clamp(dist / uDistance, 0.0, 1.0));
}

)";
}