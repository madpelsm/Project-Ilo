#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 outNormal;
	vec3 FragPos;
	vec3 color;
} gs_in[];

out VS_OUT{
	vec3 outNormal;
	vec3 color;
	vec3 FragPos;
} vs_out;



void main()
{
	vs_out.FragPos = gs_in[0].FragPos;
	vs_out.outNormal = gs_in[0].outNormal;
	vs_out.color = gs_in[0].color;
    EmitVertex();

    vs_out.FragPos = gs_in[1].FragPos;
	vs_out.outNormal = gs_in[1].outNormal;
	vs_out.color = gs_in[1].color;
    EmitVertex();

	vs_out.FragPos = gs_in[2].FragPos;
	vs_out.outNormal = gs_in[2].outNormal;
	vs_out.color = gs_in[2].color;
    EmitVertex();
    
    EndPrimitive();
}