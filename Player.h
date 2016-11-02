#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "objLoader.h"

class Player : GameObject {
private:
    std::vector<Vertex> mVertices;
    std::vector<Vertex2> mVertices2;//contains normalinfo
    std::vector<GLuint> mIndices;
    /*Shader vertShader, fragShader;
    ShaderProgram p1;*/
    GLuint shaderProgramID;
    int instances = 2;
    std::vector<glm::vec3> mOffsets;

public:

    float mX = 0, mY = 0,mZ=0, mRotAngle = 0,
        xSpd = 0.5f,ySpd=0.5f;
    glm::mat4 mTransformation = glm::mat4(1);
    GLuint mVaoPlayer, mVbo, mIbo,mInstanceVBO;
    Player();
    ~Player();
    
    void init();
    void setTransform(float x, float y, float angle);
    void setShape(std::vector<Vertex> vertices);
    void loadGeometry(std::string filePath);
    void refreshShaderTransforms();

    void fillOffsets(); //temp stuff to try out instancing
    void update();
    void createIndices();
    void createNormals();
    glm::vec3 getPosition();

    void render();
};