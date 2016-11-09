#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inMtlProps; //shininess, specStrength, AmbComp
layout (location = 4) in vec3 instanceOffset;

uniform mat4 persp;
uniform mat4 model;
uniform mat4 cam;
uniform float time;


out vec3 gPosition;
out vec3 gNormal;
out vec3 gMaterialColor;
out vec3 gMtlProps;




void main(){
	//define what you want to store vertex wise
	vec3 instanceTranslation = instanceOffset;
	vec3 vertPos = inPosition;
    if(inPosition.y<0.1f){
        vertPos = vec3(inPosition.x,inPosition.y+0.2f*sin(time+inPosition.x+inPosition.z),inPosition.z);
    }
	vec4 worldPos=model*vec4(vertPos+instanceTranslation,1.0);
	gl_Position  = persp*cam*worldPos;
	gPosition = worldPos.xyz;
	gNormal = mat3(transpose(inverse(model)))*inNormal;;
	gMaterialColor = inColor;
	gMtlProps = inMtlProps;
}