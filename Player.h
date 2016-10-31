#pragma once
#include "GameObject.h"

class Player : GameObject {
private:
    float mX, mY, mRotAngle;
    std::vector<Vertex> mVertices;
    std::vector<GLushort> mIndices;

public:
    GLuint mVao, mVbo, mIbo;
    Player();
    ~Player();
    
    void init();
    void setTransform(float x, float y, float angle);
    glm::vec3 getPosition();

    void render();
};