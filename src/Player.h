#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "objLoader.h"

class Player : GameObject {
  private:
    std::vector<Vertex> mVertices;
    std::vector<Vertex2> mVertices2; // contains normalinfo
    std::vector<GLuint> mIndices;
    /*Shader vertShader, fragShader;
  ShaderProgram p1;*/
    std::vector<glm::vec3> mOffsets;
    glm::vec3 mScale=glm::vec3(1,1,1); // default scale 1
    bool mOffsetsChanged = false;

  public:
    std::string mGeomPath = "./shapes/suzanne.obj";
    float mX = 0, mY = 0, mZ = 0, mRotAngle = 0, xSpd = 0.5f, ySpd = 0.5f;
    glm::mat4 mTransformation = glm::mat4(1);
    GLuint mVaoPlayer, mVbo, mInstanceVBO;
    Player();
    Player(std::string _geomPath);
    ~Player();
    void initGL();
    void setScale(glm::vec3 _scale);
    void setTransform(float x, float y, float z, float angleZ);
    void setShape(std::vector<Vertex> vertices);
    void loadGeometry(std::string filePath);
    void loadDefaultGeometry();
    void refreshShaderTransforms(int shaderProgramID);

    void update();
    void createIndices();
    void createNormals();
    void cleanup();
    void addInstance(glm::vec3 _offset);
    glm::vec3 getPosition();

    void render(int shaderProgramID);
};