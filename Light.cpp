#include "Light.h"
#include <iostream>


Light::Light(glm::vec3 Pos) {
    mPosition = Pos;
    mAmbientComp = 0.1f;
    mColor = glm::vec3(1, 1, 1);
}

Light::Light(glm::vec3 Pos, float AmbientComp) {
    mPosition = Pos;
    mAmbientComp = AmbientComp;
    mColor = glm::vec3(1, 1, 1);
}

Light::Light(glm::vec3 Pos, float AmbientComp, glm::vec3 Color) {
    mPosition = Pos;
    mAmbientComp = AmbientComp;
    mColor = Color;
}

Light::~Light() {

}

void Light::upload(int ShaderProgramID) {
    glUniform3f(glGetUniformLocation(ShaderProgramID, "omniLights[0].position"), mPosition.x, mPosition.y, mPosition.z);
    glUniform3f(glGetUniformLocation(ShaderProgramID, "omniLights[0].lightColor"), mColor.x, mColor.y, mColor.z);
    glUniform1f(glGetUniformLocation(ShaderProgramID, "omniLights[0].ambient"), mAmbientComp);
}

void Light::move(float dx, float dy,float dz) {
    mPosition.x = dx;
    mPosition.y = dy;
    mPosition.z = dz;


}