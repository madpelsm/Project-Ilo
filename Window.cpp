#include "Window.h"

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
    SDL_DestroyWindow(mSDLwindow);
    SDL_Quit();
}

void Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "failed to intialise video" << std::endl;
    }
    mSDLwindow = SDL_CreateWindow(mTitle.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        mWidth,mHeight,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE |SDL_WINDOW_SHOWN
    );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetSwapInterval(1);
    if (mSDLwindow == nullptr) {
        std::cout << "failed to create Window" << std::endl;
    }
    glContext = SDL_GL_CreateContext(mSDLwindow);

    windowInitialised = true;
    std::cout << "Window initialised correctly" << std::endl;

    initGL();

}

void Window::initGL() {
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    glViewport(0, 0, mWidth, mHeight);

    //load in shaders
    vertShader.loadShader("VertexShader.vert", GL_VERTEX_SHADER);
    fragShader.loadShader("FragmentShader.frag", GL_FRAGMENT_SHADER);

    //create attach link use
    p1.createProgram();
    p1.attachShaderToProgram(&vertShader);
    p1.attachShaderToProgram(&fragShader);
    p1.linkProgram();
    p1.useProgram();
    std::cout << "OpenGL window initialised" << std::endl;
    destroyShaders();

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
            closed = true;
            sdlDie();
        }
        if (event.type == SDL_WINDOWEVENT_RESIZED) {
            resize();
        }
    }
}

void Window::update() {
    int perspLoc = glGetUniformLocation(p1.getProgramID(), "persp");
    glm::mat4 perspM = glm::perspective(45.0f, mWidth / (float)mHeight, 0.1f, 100.0f); //90 degrees fov
    glProgramUniformMatrix4fv(p1.getProgramID(), perspLoc, 1, GL_FALSE, glm::value_ptr(perspM));


    int modelLoc = glGetUniformLocation(p1.getProgramID(), "model");
    glProgramUniformMatrix4fv(p1.getProgramID(), modelLoc, 1, GL_FALSE, glm::value_ptr(mPlayer.mTransformation));
    //update perspective
    //camera update

    //update gameobjects
    //box2d! pass a world

}

void Window::upload() {
    //upload data
}

void Window::render() {
    glClearColor(0, 0, 0.4*sin(SDL_GetTicks() / 1000.0) + 0.6,1);
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

    p1.deleteProgram();
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