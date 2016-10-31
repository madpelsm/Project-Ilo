#version 330
smooth in vec3 color;
void main()
{
	gl_FragColor = vec4(pow(color,vec3(0.4545) ), 1.0);
}