#pragma once
#include <fstream>
#include <glad/glad.h>
#include <stdio.h>
#include <vector>
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
class Shader {
  public:
    Shader();
    bool loadShader(std::string pos, int shaderType);
    void deleteShader();
    int getShaderID();
    bool isLoaded();

  private:
    unsigned int shaderID;
    int shaderType;
    bool loaded;
};