#include "Window.h"

int main(int argc, char * argv[]) {
    Window w(800, 600, "test");
    //set camera
    Camera c1(glm::vec3(0,3,6),glm::vec3(0,0,0),glm::vec3(0,1,0),w.p1.getProgramID());
    w.setCamera(c1);
    //set player and init him
    Player p1;
    w.setPlayer(p1);
    w.mPlayer.loadGeometry("shapes/untitled.obj");
    //set camera
    Light OmniLight(glm::vec3(0, 0, 0),0.01f,glm::vec3(1,1,1));
    w.setLight(OmniLight);
    w.initAssets();
    w.run();
    return 0;
}