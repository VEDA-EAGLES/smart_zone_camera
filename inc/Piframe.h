#ifndef PI_FRAME_H
#define PI_FRAME_H

#include <chrono>
#include <ctime>

class Piframe
{
public:
    Piframe();
    ~Piframe();
    void startCheckFrame();
    void endCheckFrame();
    float calculateFrame();

private:
    std::chrono::steady_clock::time_point Tbegin, Tend;
    float f;
    float FPS[16];
    int i, Fcnt = 0;

};

#endif // PI_FRAME_H