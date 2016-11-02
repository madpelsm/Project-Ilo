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

smooth in vec3 color;
in vec3 outNormal;
in vec3 FragPos;



uniform OmniLight omniLights[Amount_omniLights];

out vec4 FragColor;

vec3 calculateLightColor(vec3 FragPos, vec3 normal, OmniLight omniLight){
	//calculate the direction vector of the fragmentPos to the lightPosition
		vec3 lightDir = FragPos-omniLight.position;
		float distance = dot(lightDir,lightDir);
		//realistic attenuation
		float att = 1.0f/(1.0f+distance);
		//TODO set a specular similiar to phong shading
		return (omniLight.ambient+max(dot(normalize(lightDir),normal),0)*att)*omniLight.lightColor;
	
	}

void main()
{
	vec3 lightedColor;
	for(int i=0;i<Amount_omniLights;i++){
		
	lightedColor+= calculateLightColor(FragPos,outNormal,omniLights[i])*color;
	}
	FragColor = vec4(pow(lightedColor,vec3(0.4545) ), 1.0);
}