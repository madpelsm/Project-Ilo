#pragma once
#include "Shader.h"
#include <glad/glad.h>
class ShaderProgram {
  public:
    GLuint programID;
    bool bLinked;

    ShaderProgram();
    ~ShaderProgram();
    bool linkProgram();
    void useProgram();
    void createProgram();
    bool attachShaderToProgram(Shader *shader);
    int getProgramID();
    void deleteProgram();
};