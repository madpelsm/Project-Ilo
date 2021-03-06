#version 330



in vec2 texCoords;
vec3 applyKernel(float offset,float[9] kernel);
uniform sampler2D screenTex;

out vec4 color;

void main(){
	float offset = 0.1f;
	
	

    float kernel[9] = float[](
		-1,-1,-1,
        -1,9,-1,
        -1,-1,-1
    );

	//vec3 T = texture(screenTex,texCoords, 1.0).rgb;
	
    vec3 T = applyKernel(offset,kernel);
	color = vec4(pow(T,vec3(0.4545)),1.0);
}
vec3 applyKernel(float offset, float[9] kernel){
	vec3 B = vec3(0.0);
	vec2 texSize = textureSize(screenTex,0);
	vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left
        vec2(0.0,    offset),  // top-center
        vec2(offset,  offset),  // top-right
        vec2(-offset, 0.0),    // center-left
        vec2(0.0,    0.0),    // center-center
        vec2(offset,  0.0),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0,    -offset), // bottom-center
        vec2(offset,  -offset)  // bottom-right    
    );
	for(int i = 0; i < 9; i++)
    {
        B += vec3(texture(screenTex, texCoords.st + offsets[i]/texSize))*kernel[i];
    }

	return B;

}

