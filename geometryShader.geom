#version 330

layout (traingles) in;
layout (triangles, max_vertices = 3) out;

vec3 getNormal(){
	
	vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
    return normalize(cross(a, b));
}
void main(){
	gl_Position = gl_in[0].gl_Position;
    EmitVertex();
	gl_Position = gl_in[1].gl_Position;
    EmitVertex();
	gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();

}