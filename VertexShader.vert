#version 330
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 instanceOffset;

uniform mat4 persp;
uniform mat4 model;
uniform mat4 cam;

smooth out vec3 color;
out vec3 outNormal;
smooth out vec3 FragPos;

out VS_OUT{
    vec3 outNormal;
	vec3 FragPos;
	vec3 color;
} vs_out;

void main()
{
	gl_Position=persp*cam*model*vec4(inPosition+instanceOffset,1.0);
	vs_out.outNormal = mat3(transpose(inverse(model)))*inNormal;
	vs_out.color = inPosition;
	vs_out.FragPos = vec3(model*vec4(inPosition+instanceOffset,1.0f));
}
