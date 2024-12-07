#pragma once

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
    SmartZoneCamera();
    ~SmartZoneCamera();
    SmartZoneCamera(const SmartZoneCamera& ref) {}
    SmartZoneCamera& operator=(const SmartZoneCamera& ref) {}
    lccv::PiCamera cam;
    Piframe fpsInfo;
    YoloX detector;
    BYTETracker tracker;
    Area_Handler area_ctrl;
    cv::VideoWriter originalVideoWriter;
    cv::VideoWriter trackingVideoWriter;
    cv::VideoWriter udpWriter;
    int fps;
    cv::Mat frame;
    cv::VideoCapture cap;

public:
    static SmartZoneCamera& getInstance() {
        static SmartZoneCamera s;
        return s;
    }
    bool initialize();
    void processFrame();
    void run();
    void finalize();
    
};

