#include "Light.h"

Light::Light() {
    lightID = lightsCreated;
    lightsCreated++;
}

Light::Light(glm::vec3 Pos) {
    // default light with intensity 1 and ambient component of 10% will be made
    mPosition = Pos;
    mColor = glm::vec3(1, 1, 1);
    lightID = lightsCreated;
    lightsCreated++;
}

Light::Light(glm::vec3 Pos, glm::vec3 Color) {
    // set position, ambient component then the intensity
    mPosition = Pos;
    mColor = Color;
    lightID = lightsCreated;
    lightsCreated++;
}

Light::~Light() {
}
Light::Light(const Light &l) : mPosition(l.mPosition), mColor(l.mColor){
    lightID = lightsCreated;
    lightsCreated++;
}
glm::vec3 Light::getPosition() {
    return mPosition;
}

glm::vec3 Light::getColor() {
    return mColor;
}

void Light::upload(int ShaderProgramID) {
    std::string posString = "omniLights[" + std::to_string(lightID) + "].position";
    std::string lightColorString = "omniLights[" + std::to_string(lightID) + "].lightColor";
    glUniform1f(glGetUniformLocation(ShaderProgramID, "Amount_omniLights"), lightsCreated);
    glUniform3f(glGetUniformLocation(ShaderProgramID, posString.c_str()), mPosition.x, mPosition.y, mPosition.z);
    glUniform3f(glGetUniformLocation(ShaderProgramID, lightColorString.c_str()), mColor.x, mColor.y, mColor.z);
}

void Light::move(float x, float y, float z) {
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = z;
}
int Light::lightsCreated = 0;