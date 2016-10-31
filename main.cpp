#include "Window.h"

int main(int argc, char * argv[]) {
    Window w(800, 600, "test");
    Camera c1(glm::vec3(0,0,10),glm::vec3(0,0,0),glm::vec3(0,1,0),w.p1.getProgramID());
    w.setCamera(c1);
    Player p1;
    w.setPlayer(p1);
    w.mPlayer.init();
    w.run();
    return 0;
}