#include "Window.h"

Window::Window(){
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

    p1.deleteProgram();
    sdlDie();
}
void Window::sdlDie() {
    closed = true;
    SDL_DestroyWindow(mSDLwindow);
    SDL_Quit();
}

void Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "failed to intialise video" << std::endl;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    mSDLwindow = SDL_CreateWindow(mTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        mWidth, mHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
        );
    if (mSDLwindow == nullptr) {
        std::cout << "failed to create Window" << std::endl;
    }
    glContext = SDL_GL_CreateContext(mSDLwindow);
    SDL_GL_SetSwapInterval(1);
    windowInitialised = true;
    std::cout << "Window initialised correctly" << std::endl;

    initGL();

}

void Window::initAssets() {
    mPlayer.init();

}

void Window::initGL() {
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glViewport(0, 0, mWidth, mHeight);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    //face culling
    glEnable(GL_CULL_FACE);
    //discard (cull) the back facing faces and define the winding order
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);


    //load in shaders
    vertShader.loadShader("VertexShader.vert", GL_VERTEX_SHADER);
    fragShader.loadShader("FragmentShader.frag", GL_FRAGMENT_SHADER);
    //geometryShader.loadShader("geometryShader.geom", GL_GEOMETRY_SHADER);
    // add in later, a simple pass through geometry shader (can be useful later on)
    //create attach link use
    p1.createProgram();
    p1.attachShaderToProgram(&vertShader);
    //p1.attachShaderToProgram(&geometryShader);
    p1.attachShaderToProgram(&fragShader);
    p1.linkProgram();
    p1.useProgram();
    std::cout << "OpenGL window initialised" << std::endl;
    destroyShaders();
    //clear the buffer before first render, it looks nicer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(mSDLwindow);
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
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            sdlDie();
        }
        if (event.type == SDL_WINDOWEVENT_RESIZED) {
            std::cout << "resized!" << std::endl;
            resize();
        }
        if (event.type == SDL_KEYDOWN){
            std::cout << "key pressed: ";
            if (event.key.keysym.scancode == SDL_SCANCODE_A) {
                std::cout << "left";
                mCamera.mPosition.x -= mPlayer.xSpd;
                mCamera.mTarget.x -= mPlayer.xSpd;
                //mPlayer.mX -= mPlayer.xSpd;
            }
            else if (event.key.keysym.scancode == SDL_SCANCODE_D) {
                std::cout << "right";
                mCamera.mTarget.x += mPlayer.xSpd;
                mCamera.mPosition.x += mPlayer.xSpd;
                //mPlayer.mX += mPlayer.xSpd;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_W) {
                std::cout << "up";
                mCamera.mPosition.z -= mPlayer.ySpd;
                mCamera.mTarget.z -= mPlayer.ySpd;
                //mPlayer.mZ += mPlayer.ySpd;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_S) {
                std::cout << "down";
                mCamera.mPosition.z += mPlayer.ySpd;
                mCamera.mTarget.z += mPlayer.ySpd;
                //mPlayer.mZ -= mPlayer.ySpd;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_Q) {
                std::cout << "down";
                mCamera.rotate(0.01f);
                //mPlayer.mZ -= mPlayer.ySpd;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_E) {
                std::cout << "down";
                mCamera.rotate(-0.01f);
                //mPlayer.mZ -= mPlayer.ySpd;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                std::cout << "Closing";
                sdlDie();
            }

            std::cout<<"."<<std::endl;
        }
    }
}

void Window::update() {
    resize();

    //prepare the transformations
    mCamera.update();
    mPlayer.update();
    //you can put lightmovements here 
    mOmniLight.move(mCamera.mTarget.x, mCamera.mTarget.y, mCamera.mTarget.z);
    //update gameobjects
    //box2d! pass a world

}

void Window::upload() {
    //upload global values here, transformations of an object needs to happen within its ownd draw method
    //upload perspective info
    int perspLoc = glGetUniformLocation(p1.getProgramID(), "persp");
    glm::mat4 perspM = glm::perspective(45.0f, mWidth / (float)mHeight, 0.1f, 100.0f); //90 degrees fov
    glProgramUniformMatrix4fv(p1.getProgramID(), perspLoc, 1, GL_FALSE, glm::value_ptr(perspM));

    //upload camera info
    mCamera.uploadCameraInfo();

    //set modelTransform to unity as start
    int modelLoc = glGetUniformLocation(p1.getProgramID(), "model");
    glProgramUniformMatrix4fv(p1.getProgramID(), modelLoc, 1, GL_FALSE, glm::value_ptr(mPlayer.mTransformation));

    //upload light info
    mOmniLight.upload(p1.getProgramID());

}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mPlayer.render();


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
    glViewport(0, 0, mWidth, mHeight);
}

void Window::destroyShaders(){

    vertShader.deleteShader();
    fragShader.deleteShader();
}

void Window::setCamera(Camera c) {
    mCamera = c;
}

void Window::setPlayer(Player p) {
    //set the player for this game
    mPlayer = p;

}

void Window::setLight(Light light) {
    mOmniLight = light;
}