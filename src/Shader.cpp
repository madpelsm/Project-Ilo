#include "Shader.h"
#include <iostream>

Shader::Shader() {
}
bool Shader::loadShader(std::string pos, int shaderType) {
    std::ifstream t(pos);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    shaderID = glCreateShader(shaderType);
    const char *c_str = str.c_str();
    if (shaderID == 0) {
        std::cout << "error when creating shader" << std::endl;
    }
    glShaderSource(shaderID, 1, &c_str, nullptr);
    glCompileShader(shaderID);

    // debugging
    GLint isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        // Provide the infolog in whatever manor you deem best.
        std::cout << "GLSL Shader error:" << std::endl;
        for (unsigned int i = 0; i < errorLog.size(); i++) {
            std::cout << errorLog[i];
        }
        // Exit with failure.
        glDeleteShader(shaderID); // Don't leak the shader.
        return false;
    }
    this->shaderType = shaderType;
    loaded = true;
    return 1;
}

int Shader::getShaderID() {
    return shaderID;
}
bool Shader::isLoaded() {
    return loaded;
}

void Shader::deleteShader() {
    if (!loaded)
        return;
    loaded = false;
    glDeleteShader(shaderID);
}
