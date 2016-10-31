#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ShaderProgram.h"

class Player : GameObject {
private:
    float mX=0, mY=0, mRotAngle=0;
    std::vector<Vertex> mVertices;
    std::vector<GLushort> mIndices;
    std::vector<glm::vec3> mNormals;
    Shader vertShader, fragShader;
    ShaderProgram p1;


public:

    glm::mat4 mTransformation = glm::mat4(1);
    GLuint mVaoPlayer, mVbo, mIbo,mNormID;
    Player();
    ~Player();
    
    void init();
    void setTransform(float x, float y, float angle);
    void setShape(std::vector<Vertex> vertices);

    void createIndices();
    void createNormals();
    glm::vec3 getPosition();

    void render();
};