#pragma once

#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include "yoloX.h"
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
    std::vector<People_count> get_peoplecount();
    std::vector<People_stay> get_stay();
    std::vector<People_move> get_move();
};

