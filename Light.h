#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

class Light {
    glm::vec3 mPosition;
    glm::vec3 mColor;
    float mAmbientComp;
public:
    Light() {

    }
    Light(glm::vec3 Pos);
    Light(glm::vec3 Pos, float amb);
    Light(glm::vec3 Pos, float amb,glm::vec3 Color);
    ~Light();
    void upload(int ShaderProgramID);
    void move(float dx, float dy,float dz);

};