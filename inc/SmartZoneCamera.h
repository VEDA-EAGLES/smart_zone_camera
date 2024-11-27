#ifndef SMART_ZONE_CAMERA_H
#define SMART_ZONE_CAMERA_H

#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include "yoloX.h"
#include "Piframe.h"
#include <chrono>
#include <ctime>
#include <vector>

class SmartZoneCamera {
private:
    lccv::PiCamera cam;
    Piframe fpsInfo;
    YoloX detector;
    BYTETracker tracker;
    cv::VideoWriter originalVideoWriter;
    cv::VideoWriter trackingVideoWriter;
    int fps;
    cv::Mat frame;

public:
    SmartZoneCamera();
    ~SmartZoneCamera();

    bool initialize();
    void processFrame();
    void run();
    void finalize();
};

#endif // SMART_ZONE_CAMERA_H
