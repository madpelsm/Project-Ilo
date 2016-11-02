#pragma once
#include <SDL.h>
#include <iostream>
#include <glad\glad.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include <glm.hpp>
#include "glm\glm\gtc\matrix_transform.hpp"
#include "glm\glm\gtc\type_ptr.hpp"
#include "Camera.h"
#include "GameObject.h"
#include "Player.h"
#include "Light.h"

class Window {
    int mWidth, mHeight;
    std::string mTitle;
    bool closed = false;
    bool windowInitialised = false;
    short frames;
    float lastTime = SDL_GetTicks();
public:
    Camera mCamera;
    ShaderProgram p1;
    SDL_Window * mSDLwindow=nullptr;
    SDL_GLContext glContext;
    SDL_Event event;
    Shader vertShader, fragShader,geometryShader;
    std::vector<GameObject> mGameObjects;
    Player mPlayer;
    Light mOmniLight;


    Window();
    Window(int width, int height, std::string title);
    ~Window();
    void sdlDie();
    void init();
    void initGL();
    void initAssets();
    void run();
    void update();
    void upload();
    void render();
    void checkEvents();
    void resize();
    void destroyShaders();

    void setCamera(Camera c);
    void setPlayer(Player p);
    void setLight(Light light);
};