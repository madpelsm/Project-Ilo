#version 330



struct OmniLight{
	vec3 position;
	vec3 lightColor;
};

in vec2 TexCoords;

vec3 calculateLightColor(vec3 FragPos, vec3 normal, OmniLight omniLight);
#define Amount_omniLights 7


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gMtlColor;
uniform sampler2D gMtlProp;
uniform OmniLight omniLights[Amount_omniLights];
uniform vec3 eyePos;

out vec3 FragColor;

void main()
{
	vec3 lightedSurface=vec3(0,0,0);
	vec3 FragPos = texture(gPosition,TexCoords).rgb;
	vec3 outNormal = texture(gNormal,TexCoords).rgb;
	vec3 color = texture(gMtlColor,TexCoords).rgb;

	for(int i=0;i<Amount_omniLights;i++){
		lightedSurface+= calculateLightColor(FragPos,outNormal,omniLights[i])*color;
	}

	
	
	FragColor =lightedSurface;

}
vec3 calculateLightColor(vec3 FragPos, vec3 normal, OmniLight omniLight){
	//set specular amount
	
	vec3 lightDir = vec3(FragPos-omniLight.position);
	vec3 viewDir = normalize(eyePos-FragPos);
	vec3 reflectDir = normalize(reflect(lightDir,normal));

	//shininess and specStrength should become material dependend values
	float shininess = texture(gMtlProp,TexCoords).r;
	float specStrength = texture(gMtlProp,TexCoords).g;
	float ambient = texture(gMtlProp,TexCoords).b;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float specularBit = spec*specStrength;
	

	//calculate the direction vector of the fragmentPos to the lightPosition
		float distance = dot(lightDir,lightDir);
		//realistic attenuation
		float att = 1.0f/(1.0f+distance);
		float diffuseBit = max(dot(normalize(lightDir),normal),0);
		return att*(specularBit+ambient+diffuseBit)*omniLight.lightColor;
	}