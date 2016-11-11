#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "Player.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Window {
    int mWidth, mHeight;
    std::string mTitle;
    bool closed = false;
    bool windowInitialised = false;
    short frames;
    bool vSync = true,fullscreen = false,windowMaximised = false;
    float lastTime = SDL_GetTicks(),
          walkAroundSpeed = 0.09f // camera movement speed
        ,
          rotateSpeed = 0.01f, mSSAA_amount = 2.0f; // camera rotation speed
  public:
    Camera mCamera;
    ShaderProgram firstPassShader, secondPassShader, thirdPassShader;
    SDL_Window *mSDLwindow = nullptr;
    SDL_GLContext glContext;
    SDL_Event event;
    Shader vertShader, fragShader, firstPassVertShader, firstPassFragShader, secondPassVertShader, secondPassFragShader,
        thirdPassVertShader, thirdPassFragShader;
    std::vector<Player *> mGameObjects;
    std::vector<Light *> mOmniLights;
    GLuint gbuffer, gPosition, gNormal, gMaterialColor, gMaterialProps, rboDepth, quadVao, ppFBO, ppRBO, screenTex;
    GLuint attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};

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
    void renderFirstPass();
    void renderSecondPass();
    void renderThirdPass();
    void checkEvents();
    void resize();
    void destroyShaders();
    void loadGeometries();
    void initQuadMesh();
    void drawQuad();
    void prepareForDeferredShading();
    void preparePostProcessing();
    void setvSync(bool vSyncStatus);
    void setSSAA(float _SSAAamount);

    void setCamera(Camera &c);
    void addNPC(Player &npc);
    void setPlayer(Player &p);
    void setLight(Light &light);
};