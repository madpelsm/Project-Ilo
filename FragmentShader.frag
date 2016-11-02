#version 330

//hold the omnilights in an array of OmniLight structs
struct OmniLight{
//light struct containing the position, ambient amount and the color
	vec3 position;
	vec3 lightColor;
	float ambient;

	

};

vec3 calculateLightColor(vec3 FragPos, vec3 normal, OmniLight omniLight);
#define Amount_omniLights 1


in VS_OUT {
	vec3 outNormal;
	vec3 FragPos;
	vec3 color;
} vs_in;


uniform OmniLight omniLights[Amount_omniLights];
uniform vec3 eyePos;

out vec4 FragColor;

vec3 calculateLightColor(vec3 FragPos, vec3 normal, OmniLight omniLight){
	//set specular amount
	
	vec3 lightDir = FragPos-omniLight.position;
	vec3 viewDir = normalize(eyePos-FragPos);
	vec3 reflectDir = reflect(lightDir,normal);

	float spec = pow(max(dot(viewDir, normalize(reflectDir)), 0.0), 32);
	float specStrength = 0.1f;

	float specularBit = spec*specStrength;
	

	//calculate the direction vector of the fragmentPos to the lightPosition
		float distance = dot(lightDir,lightDir);
		//realistic attenuation
		float att = 1.0f/(1.0f+distance);
		float diffuseBit = max(dot(normalize(lightDir),normal),0)*att;
		//TODO set a specular similiar to phong shading
		return (specularBit+omniLight.ambient+diffuseBit)*omniLight.lightColor;
	
	}

void main()
{
	vec3 lightedColor;
	for(int i=0;i<Amount_omniLights;i++){
		
	lightedColor+= calculateLightColor(vs_in.FragPos,vs_in.outNormal,omniLights[i])*vs_in.color;
	}
	FragColor = vec4(pow(lightedColor,vec3(0.4545) ), 1.0);
}