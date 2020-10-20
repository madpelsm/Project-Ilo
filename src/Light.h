#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
class Light {
    glm::vec3 mPosition;
    glm::vec3 mColor;

  public:
    int lightID;
    static int lightsCreated; // tells the number of lights so fa rcreated
    Light();
    Light(const Light &l);
    Light(glm::vec3 Pos);
    Light(glm::vec3 Pos, glm::vec3 Color);
    ~Light();
    void upload(int ShaderProgramID);
    glm::vec3 getPosition();
    glm::vec3 getColor();
    void move(float dx, float dy, float dz);
};