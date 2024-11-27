#include "Piframe.h"

Piframe::Piframe() {
    // 생성자 구현
}

Piframe::~Piframe() {
    // 소멸자 구현
}


void Piframe::startCheckFrame() {
    Tbegin = std::chrono::steady_clock::now();
}

void Piframe::endCheckFrame() {
    Tend = std::chrono::steady_clock::now();
}

float Piframe::calculateFrame() {
    f = std::chrono::duration_cast<std::chrono::milliseconds>(Tend - Tbegin).count();
    if (f > 0.0) FPS[((Fcnt++) & 0x0F)] = 1000.0 / f;
    for(f = 0.0, i = 0; i < 16; i++) { f += FPS[i]; }
    return f;
}