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
    SmartZoneCamera& operator=(const SmartZoneCamera& ref) {return *this;}
    Piframe fpsInfo;
    YoloX detector;
    BYTETracker tracker;
    int fps;

public:
    static SmartZoneCamera& getInstance() {
        static SmartZoneCamera s;
        return s;
    }
    void init();
    cv::Mat processFrame(cv::Mat& frame);
    Area_Handler area_ctrl;
};

