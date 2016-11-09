#include "Window.h"
int main(int argc, char * argv[]) {
    Window w(800, 600, "Project Ilo");
    w.setvSync(false);
    w.setSSAA(1.0);
    //set camera
    //Position, then target then define the up direction (+Y = Up)
    float eyeHeight = 2.0f;
    Camera c1(glm::vec3(0,eyeHeight,0),glm::vec3(0,eyeHeight,-1.5f),glm::vec3(0,1,0));
    w.setCamera(c1);
    //set player and init him
    Player * p1 = new Player("./shapes/untitled.obj");
    //this adds to mGameobjects (bad naming )

    w.addNPC(*p1);
    (*p1).setInstances(1);

    Player * light = new Player("./shapes/untitled.obj");
    w.addNPC(*light);
    (*light).setTransform(0, 8.0f, 0.0f, 3.14f);



    //create lights and add them to the window
    //the first light created will be bound on the camera's position
    Light OmniLight(glm::vec3(0, 2, 2),  glm::vec3(0.2f,0.2f,0.2f));
    w.setLight(OmniLight); 
    //orange light
    Light OmniLight2(glm::vec3(0, 2, 2),  glm::vec3(1, 0.6, 0.2));
    w.setLight(OmniLight2);
    //purple light
    Light OmniLight3(glm::vec3(6, 2, 2),  glm::vec3(0.6, 0.3, 0.9));
    w.setLight(OmniLight3);
    //blue light
    Light OmniLight4(glm::vec3(-8, 2, 2),  glm::vec3(0.3, 0.3, 1));
    w.setLight(OmniLight4);
    //green light
    Light OmniLight5(glm::vec3(-6, 2, -6),  glm::vec3(0.3, 1, 0.3));
    w.setLight(OmniLight5);
    //yellow light
    Light OmniLight6(glm::vec3(6, 2, -6),  glm::vec3(1, 1, 0.3));
    w.setLight(OmniLight6);
    //red
    Light OmniLight7(glm::vec3(0, 2, -6),  glm::vec3(1, .2, 0.2));
    w.setLight(OmniLight7);
    //dont forget to set Amount_omniLights in the fragShader


    w.loadGeometries();
    w.initAssets();
    w.run();
    return 0;
}