#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "objLoader.h"

class Player : GameObject {
private:
    float mX=0, mY=0, mRotAngle=0;
    std::vector<Vertex> mVertices;
    std::vector<Vertex2> mVertices2;//contains normalinfo
    std::vector<GLushort> mIndices;
    Shader vertShader, fragShader;
    ShaderProgram p1;


public:

    glm::mat4 mTransformation = glm::mat4(1);
    GLuint mVaoPlayer, mVbo, mIbo;
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