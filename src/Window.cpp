#include "Window.h"

#include <thread>
Window::Window() {
    mWidth = 800;
    mHeight = 600;
    mTitle = "untitled";
}

Window::Window(int width, int height, std::string title) {
    mWidth = width;
    mHeight = height;
    mTitle = title;
    std::cout << "Window with parameters set" << std::endl;
    init();
}

Window::~Window() {
    sdlDie();
}
void Window::sdlDie() {
    closed = true;
    std::cout << "killing SDL" << std::endl;
    for (unsigned int i = 0; i < mGameObjects.size(); i++) {
        mGameObjects[i]->cleanup();
    }
    glDeleteFramebuffers(1, &gbuffer);
    glDeleteTextures(1, &gPosition);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gMaterialColor);
    glDeleteTextures(1, &gMaterialProps);
    glDeleteVertexArrays(1, &quadVao);
    glDeleteRenderbuffers(1, &rboDepth);
    glDeleteRenderbuffers(1, &ppRBO);
    glDeleteFramebuffers(1, &ppFBO);

    thirdPassShader.deleteProgram();
    secondPassShader.deleteProgram();
    firstPassShader.deleteProgram();
    SDL_DestroyWindow(mSDLwindow);
    SDL_Quit();
}

void Window::preparePostProcessing() {
    glGenFramebuffers(1, &ppFBO);
    // glGenRenderbuffers(1, &ppRBO);
    // gen texture
    glGenTextures(1, &screenTex);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, ppFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTex, 0);

    GLenum attch[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, attch);

    glBindRenderbuffer(GL_RENDERBUFFER, ppRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ppRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Post processing fbo not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "failed to intialise video" << std::endl;
    }
    -SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    mSDLwindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (mSDLwindow == nullptr) {
        std::cout << "failed to create Window" << std::endl;
    }
    glContext = SDL_GL_CreateContext(mSDLwindow);
    // vsync
    SDL_GL_SetSwapInterval(1);
    windowInitialised = true;
    std::cout << "Window initialised correctly" << std::endl;

    initGL();
    prepareForDeferredShading();
    initQuadMesh();
}

void Window::setvSync(bool vSyncStatus) {
    vSync = vSyncStatus;
}

void Window::loadGeometries() {
    std::vector<std::thread> loaders;
    for (unsigned int i = 0; i < mGameObjects.size(); i++) {
        loaders.push_back(std::thread(&Player::loadDefaultGeometry, mGameObjects[i]));
    }

    for (unsigned int i = 0; i < loaders.size(); i++) {
        loaders[i].join();
    }
    loaders.clear();
}

void Window::initAssets() {
    for (unsigned int i = 0; i < mGameObjects.size(); i++) {
        mGameObjects[i]->initGL();
    }
}

void Window::initGL() {
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    // glViewport(0, 0, mWidth, mHeight);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    // face culling
    glEnable(GL_CULL_FACE);
    // discard (cull) the back facing faces and define the winding order
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // first pass shader creation
    firstPassVertShader.loadShader("./shaders/firstPassVertex.vert", GL_VERTEX_SHADER);
    firstPassFragShader.loadShader("./shaders/firstPassFragment.frag", GL_FRAGMENT_SHADER);
    firstPassShader.createProgram();
    firstPassShader.attachShaderToProgram(&firstPassVertShader);
    firstPassShader.attachShaderToProgram(&firstPassFragShader);
    firstPassShader.linkProgram();
    firstPassShader.useProgram();

    secondPassVertShader.loadShader("./shaders/secondPassVert.vert", GL_VERTEX_SHADER);
    secondPassFragShader.loadShader("./shaders/secondPassFrag.frag", GL_FRAGMENT_SHADER);

    secondPassShader.createProgram();
    secondPassShader.attachShaderToProgram(&secondPassVertShader);
    secondPassShader.attachShaderToProgram(&secondPassFragShader);
    secondPassShader.linkProgram();

    thirdPassVertShader.loadShader("./shaders/thirdPassVert.vert", GL_VERTEX_SHADER);
    thirdPassFragShader.loadShader("./shaders/thirdPassFrag.frag", GL_FRAGMENT_SHADER);

    thirdPassShader.createProgram();
    thirdPassShader.attachShaderToProgram(&thirdPassVertShader);
    thirdPassShader.attachShaderToProgram(&thirdPassFragShader);
    thirdPassShader.linkProgram();
    thirdPassShader.useProgram();
    glUniform1i(glGetUniformLocation(thirdPassShader.getProgramID(), "screenTex"), 0);

    destroyShaders();
    secondPassShader.useProgram();
    glUniform1i(glGetUniformLocation(secondPassShader.getProgramID(), "gPosition"), 0);
    glUniform1i(glGetUniformLocation(secondPassShader.getProgramID(), "gNormal"), 1);
    glUniform1i(glGetUniformLocation(secondPassShader.getProgramID(), "gMtlColor"), 2);
    glUniform1i(glGetUniformLocation(secondPassShader.getProgramID(), "gMtlProp"), 3);

    std::cout << "OpenGL window initialised" << std::endl;
    // clear the buffer before first render, it looks nicer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(mSDLwindow);
}

void Window::prepareForDeferredShading() {
    glGenFramebuffers(1, &gbuffer);
    glGenRenderbuffers(1, &rboDepth);
    // generate texturebuffer only once, otherwise mem leak
    glGenTextures(1, &gPosition);
    glGenTextures(1, &gNormal);
    glGenTextures(1, &gMaterialColor);
    glGenTextures(1, &gMaterialProps);

    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
    // position
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // normals
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16_SNORM, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // materialColor
    glBindTexture(GL_TEXTURE_2D, gMaterialColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gMaterialColor, 0);
    // materialProps
    glBindTexture(GL_TEXTURE_2D, gMaterialProps);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMaterialProps, 0);

    // draw to the textures
    glDrawBuffers(4, attachments);
    // bind the depthbuffer

    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mWidth, mHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    } else {
        std::cout << "framebuffer Complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    preparePostProcessing();
}

void Window::initQuadMesh() {
    GLfloat quadVertices[] = {
        // Positions        // Texture Coords
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // Setup plane VAO
    glGenVertexArrays(1, &quadVao);
    glGenBuffers(1, &quadVao);
    glBindVertexArray(quadVao);
    glBindBuffer(GL_ARRAY_BUFFER, quadVao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
}

void Window::run() {
    if (!windowInitialised) {
        std::cout << "Window not initialised" << std::endl;
        return;
    }
    while (!closed) {
        checkEvents();
        update();
        upload();
        render();
    }
}

void Window::checkEvents() {
    SDL_PumpEvents();
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // for multiple keyinputs at once
    if (state[SDL_SCANCODE_A]) {
        std::cout << "left" << std::endl;
        mCamera.movePerpendicularOnDir(-walkAroundSpeed);
        /*mCamera.mPosition.x -= mPlayer.xSpd;
    mCamera.mTarget.x -= mPlayer.xSpd;*/
        // mPlayer.mX -= mPlayer.xSpd;
    }
    if (state[SDL_SCANCODE_D]) {
        std::cout << "right" << std::endl;
        mCamera.movePerpendicularOnDir(walkAroundSpeed);
        /*mCamera.mTarget.x += mPlayer.xSpd;
    mCamera.mPosition.x += mPlayer.xSpd;*/
        // mPlayer.mX += mPlayer.xSpd;
    }
    if (state[SDL_SCANCODE_W]) {
        std::cout << "up" << std::endl;
        mCamera.moveWithDir(walkAroundSpeed);
        /*mCamera.mPosition.z -= mPlayer.ySpd;
    mCamera.mTarget.z -= mPlayer.ySpd;*/
        // mPlayer.mZ += mPlayer.ySpd;
    }
    if (state[SDL_SCANCODE_S]) {
        std::cout << "down" << std::endl;
        mCamera.moveWithDir(-walkAroundSpeed);
        /*mCamera.mPosition.z += mPlayer.ySpd;
    mCamera.mTarget.z += mPlayer.ySpd;*/
        // mPlayer.mZ -= mPlayer.ySpd;
    }
    if (state[SDL_SCANCODE_Q]) {
        std::cout << "down" << std::endl;
        mCamera.rotateJaw(rotateSpeed);
        // mPlayer.mZ -= mPlayer.ySpd;
    }
    if (state[SDL_SCANCODE_E]) {
        std::cout << "down" << std::endl;
        mCamera.rotateJaw(-rotateSpeed);
        // mPlayer.mZ -= mPlayer.ySpd;
    }
    if (state[SDL_SCANCODE_ESCAPE]) {
        std::cout << "Closing" << std::endl;
        sdlDie();
    }
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                resize();
                std::cout << "resized" << std::endl;
                break;
            }
            break;
        case SDL_QUIT:
            sdlDie();
            break;
        case SDL_FINGERMOTION:
            mCamera.rotateJaw((-10) * event.tfinger.dx);
            mCamera.rotatePitch((-10) * event.tfinger.dy);
            break;
        case SDL_MOUSEMOTION:
            mCamera.rotateJaw((-1) * rotateSpeed * event.motion.xrel);
            mCamera.rotatePitch((-1) * rotateSpeed * event.motion.yrel);
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_RETURN:
                if (event.key.keysym.mod & KMOD_LALT) {
                    if (!fullscreen) {
                        fullscreen = true;
                        SDL_SetWindowFullscreen(mSDLwindow, 1);
                    }
                    else {
                        fullscreen = false;
                        SDL_SetWindowFullscreen(mSDLwindow, 0);
                    }
                }
                break;
            case SDL_SCANCODE_F11:
                if (!windowMaximised) {
                    windowMaximised = true;
                    SDL_MaximizeWindow(mSDLwindow);
                }
                else {
                    windowMaximised = false;
                    SDL_RestoreWindow(mSDLwindow);
                }
            }
            break;
        }
    }
}

void Window::update() {
    // prepare the transformations
    mCamera.update();
    // mPlayer.update();
    for (unsigned int i = 0; i < mGameObjects.size(); i++) {
        mGameObjects[i]->update();
    }

    // you can put lightmovements here

    mOmniLights[0]->move(mCamera.mTarget.x, mCamera.mTarget.y, mCamera.mTarget.z);
    mGameObjects[1]->setTransform(mCamera.mTarget.x, mCamera.mTarget.y, mCamera.mTarget.z,0);
    // update gameobjects
    // box2d! pass a world
}

void Window::upload() {
    // upload global values here, transformations of an object needs to happen
    // within its ownd draw method
    // upload perspective info
    firstPassShader.useProgram();
    int perspLoc = glGetUniformLocation(firstPassShader.getProgramID(), "persp");
    glm::mat4 perspM = glm::perspective(1.4f, mWidth / (float)mHeight, 0.1f, 50.0f); // 90 degrees fov
    glProgramUniformMatrix4fv(firstPassShader.getProgramID(), perspLoc, 1, GL_FALSE, glm::value_ptr(perspM));

    // set modelTransform to unity as start
    int modelLoc = glGetUniformLocation(firstPassShader.getProgramID(), "model");
    glProgramUniformMatrix4fv(firstPassShader.getProgramID(), modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

    // upload camera info
    mCamera.uploadCameraInfo(firstPassShader.getProgramID());

    secondPassShader.useProgram();
    mCamera.setViewPos(secondPassShader.getProgramID());
    for (auto i = 0; i < mOmniLights.size(); i++) {
        mOmniLights[i]->upload(secondPassShader.getProgramID());
    }
}

void Window::renderFirstPass() {
    // set the shaderProgram
    glBindFramebuffer(GL_FRAMEBUFFER, gbuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    firstPassShader.useProgram();
    for (unsigned int i = 0; i < mGameObjects.size(); i++) {
        (*mGameObjects[i]).render(firstPassShader.getProgramID());
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Window::renderSecondPass() {
    glDisable(GL_CULL_FACE);
    glBindFramebuffer(GL_FRAMEBUFFER, ppFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    secondPassShader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gMaterialColor);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gMaterialProps);
    drawQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // light stuff
    // upload light info
}

void Window::renderThirdPass() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    thirdPassShader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glGenerateMipmap(GL_TEXTURE_2D);
    drawQuad();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Window::drawQuad() {
    glBindVertexArray(quadVao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Window::render() {
    // DEFERRED SHADING
    // first pass: pass position, normals, material Color, spec and ambient info
    // in textures
    // Position: position
    // Normal : normal
    // MaterialColor: material Color (R,G,B)
    // MaterialProps: MaterialProps.x = Shininess, MaterialProps.y = specStrenght,
    // MaterialProps.z = ambient amount
    // first pass: create the textures

    // enable the framebuffer before rendering the first pass
    // render all objects in the first pass
    renderFirstPass();
    // second pass: render with the textures
    renderSecondPass();
    renderThirdPass();

    SDL_GL_SwapWindow(mSDLwindow);
    float currentTime = SDL_GetTicks();
    frames++;
    if (currentTime - lastTime >= 1000) {
        std::cout << frames << std::endl;
        frames = 0;
        lastTime += 1000;
    }
}

void Window::resize() {
    SDL_GetWindowSize(mSDLwindow, &mWidth, &mHeight);
    // clean up previous buffers
    glDeleteFramebuffers(1, &gbuffer);
    glDeleteTextures(1, &gPosition);
    glDeleteTextures(1, &gMaterialColor);
    glDeleteTextures(1, &gMaterialProps);
    glDeleteTextures(1, &screenTex);
    glDeleteRenderbuffers(1, &ppRBO);
    glDeleteFramebuffers(1, &ppFBO);
    glDeleteRenderbuffers(1, &rboDepth);
    prepareForDeferredShading();
    glViewport(0, 0, mWidth, mHeight);
}

void Window::destroyShaders() {
    firstPassVertShader.deleteShader();
    firstPassFragShader.deleteShader();
    secondPassFragShader.deleteShader();
    secondPassVertShader.deleteShader();
    thirdPassVertShader.deleteShader();
    thirdPassFragShader.deleteShader();
    vertShader.deleteShader();
    fragShader.deleteShader();
}

void Window::setCamera(Camera &c) {
    mCamera = c;
}

void Window::setSSAA(float _SSAAamount) {
    if (_SSAAamount < 1.0f) {
        mSSAA_amount = 1.0f;
    } else {
        mSSAA_amount = _SSAAamount;
    }
}

void Window::setPlayer(Player &p) {
    // set the player for this game
    mGameObjects.push_back(&p);
}

void Window::setLight(Light &light) {
    mOmniLights.push_back(&light);
}

void Window::addNPC(Player &npc) {
    this->mGameObjects.push_back(&npc);
}