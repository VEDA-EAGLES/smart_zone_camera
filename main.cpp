#include "SmartZoneCamera.h"
#include <iostream>

int main(int argc, char** argv) {
    SmartZoneCamera camera;

    if (!camera.initialize()) {
        std::cerr << "Failed to initialize the camera system." << std::endl;
        return -1;
    }

    camera.run();
    return 0;
}
