#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>
#include <string>
class Light {
    glm::vec3 mPosition;
    glm::vec3 mColor;
    int lightID;
public:
    static int lightsCreated; //tells the number of lights so fa rcreated
    Light() {

    }
    Light(glm::vec3 Pos);
    Light(glm::vec3 Pos, glm::vec3 Color);
    ~Light();
    void upload(int ShaderProgramID);
    void move(float dx, float dy, float dz);

};