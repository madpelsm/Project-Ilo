#include "Light.h"


Light::Light(glm::vec3 Pos) {
    //default light with intensity 1 and ambient component of 10% will be made
    mPosition = Pos;
    mColor = glm::vec3(1, 1, 1);
    lightID = lightsCreated;
    lightsCreated++;
}


Light::Light(glm::vec3 Pos, glm::vec3 Color) {
    //set position, ambient component then the intensity
    mPosition = Pos;
    mColor = Color;
    lightID = lightsCreated;
    lightsCreated++;
}

Light::~Light() {

}

void Light::upload(int ShaderProgramID) {
    std::string posString = "omniLights[" + std::to_string(lightID) + "].position";
    std::string lightColorString = "omniLights[" + std::to_string(lightID) + "].lightColor";
    glUniform3f(glGetUniformLocation(ShaderProgramID, posString.c_str()), mPosition.x, mPosition.y, mPosition.z);
    glUniform3f(glGetUniformLocation(ShaderProgramID, lightColorString.c_str()), mColor.x, mColor.y, mColor.z);
}

void Light::move(float dx, float dy,float dz) {
    mPosition.x = dx;
    mPosition.y = dy;
    mPosition.z = dz;


}
int Light::lightsCreated = 0;