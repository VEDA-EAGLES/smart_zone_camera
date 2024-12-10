#ifndef SMART_ZONE_CAMERA_H
#define SMART_ZONE_CAMERA_H

#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include "yoloX.h"
#include "Piframe.h"
#include "area.h"
#include <chrono>
#include <ctime>
#include <vector>

class SmartZoneCamera {
private:
    lccv::PiCamera cam;
    cv::VideoCapture cap;
    Piframe fpsInfo;
    YoloX detector;
    BYTETracker tracker;
    Area_Handler area_ctrl;
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

    std::vector<People_count> get_peoplecount();
    std::vector<People_stay> get_stay();
    std::vector<People_move> get_move();
};

#endif // SMART_ZONE_CAMERA_H
