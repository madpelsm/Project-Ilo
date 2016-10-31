#version 330
layout (location =0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location =2) in vec3 inNormal;

uniform mat4 persp;
uniform mat4 model;

smooth out vec3 color;
out vec3 outNormal;

void main()
{
	gl_Position=persp*model*vec4(inPosition,1.0);
	outNormal = mat3(transpose(inverse(model)))*inNormal;
	color = inColor;
}