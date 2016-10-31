#pragma once
#include <SDL.h>
#include <iostream>
#include <glad\glad.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include <glm.hpp>
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"

class Window {
    int mWidth, mHeight;
    std::string mTitle;
    bool closed = false;
    bool windowInitialised = false;
    short frames;
public:

    ShaderProgram p1;
    SDL_Window * mSDLwindow=nullptr;
    SDL_GLContext glContext;
    SDL_Event event;
    Shader vertShader, fragShader;


    Window();
    Window(int width, int height, std::string title);
    ~Window();
    void sdlDie();
    void init();
    void initGL();
    void run();
    void update();
    void upload();
    void render();
    void checkEvents();
    void resize();
    void destroyShaders();
};