#version 330
layout (location = 0) out vec3 gPositionOut;
layout (location = 1) out vec3 gNormalOut;
layout (location = 2) out vec3 gMtlColor;
layout (location = 3) out vec3 gMtlProp;

in vec3 gPosition;
in vec3 gNormal;
in vec3 gMaterialColor;
in vec3 gMtlProps;
void main()
{    
	//set the out values, set the gluniform1i for the out vec3's to the right 
	// location (glUniform1i(...))
    // Store the fragment position vector in the first gbuffer texture
    gPositionOut = gPosition;
    // store normal
    gNormalOut = gNormal;
    // store mtl color
	gMtlColor = gMaterialColor;
    // Store mtl properties 
	gMtlProp=gMtlProps;
}