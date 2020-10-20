#pragma once
#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "Player.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

inline void _glCheckError(const char *file, int line) {
    GLenum err;
    while ((err = glGetError()) != 0) {
        printf("GL Error %x @%s:%d\n", err, file, line);
    }
}

#define glCheckError() _glCheckError(__FILE__, __LINE__)

class Window {
    int mWidth, mHeight, baseObjects = 0;
    std::string mTitle;
    bool closed = false;
    bool windowInitialised = false;
    short frames;
    bool vSync = true,fullscreen = false,windowMaximised = false;
    float lastTime = SDL_GetTicks(),
        walkAroundSpeed = 0.09f // camera movement speed
        ,
        mMouseSensitivity = 0.01f // camera rotation speed
        , mSSAA_amount = 2.0f
        , mSrollSensitivity = 0.5f
        , mFOV = 1.4f //vertical field of view in radians 
        , selectedObj = 1, previouslySelected = 0;
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
    void setMouseSensitivity(float _sensitivity);
    void setScrollSensitivity(float _sensitivity);
    void setFOV(float _fov);
    void removeAllPlacedLights();
    void removeLastPlacedLight();
    void removeLastPlacedObj();

    void setCamera(Camera &c);
    void addNPC(Player &npc);
    void setPlayer(Player &p);
    void setLight(Light &light);
};
