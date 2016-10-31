#version 330
layout (location =0) in vec3 inPosition;

uniform mat4 persp;

smooth out vec3 color;

void main()
{
	gl_Position=vec4(inPosition,1.0);
	color = vec3(0,1,0);
}