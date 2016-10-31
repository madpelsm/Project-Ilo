#include "Window.h"

int main(int argc, char * argv[]) {
    Window w(800, 600, "test");
    w.init();
    w.run();
    return 0;
}