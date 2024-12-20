#include "SmartZoneCamera.h"
#include "area.h"
#include <iostream>

SmartZoneCamera::SmartZoneCamera()
    : fps(10), tracker(fps, 10) {}

SmartZoneCamera::~SmartZoneCamera() {}

std::chrono::system_clock::time_point last_video_event_time = std::chrono::system_clock::now();

cv::Mat SmartZoneCamera::processFrame(cv::Mat& frame) {

    // Object detection
    std::vector<Object> objects;
    detector.detect_yolox(frame, objects);
    objects.erase(std::remove_if(objects.begin(), objects.end(), [](const Object& obj) {return obj.label != 0;}), objects.end());
    // Object tracking
    std::vector<STrack> output_stracks = tracker.update(objects);
    
    area_ctrl.update(frame);
    // Draw tracking results
    for (size_t i = 0; i < output_stracks.size(); i++) {
        std::vector<float> tlwh = output_stracks[i].tlwh;
        Area a(tlwh[0],tlwh[1],tlwh[2],tlwh[3]);
        if(area_ctrl.area_within(a,output_stracks[i].track_id)){
            area_ctrl.draw_area(frame,a,output_stracks[i].track_id);
        }
    }

    return frame;
}

void SmartZoneCamera::init()
{
    detector.init("../model/yoloxN.param", "../model/yoloxN.bin");
}

std::vector<People_count> SmartZoneCamera::get_peoplecount() {
    return area_ctrl.calc_peoplecount();
}

std::vector<People_stay> SmartZoneCamera::get_stay() {
    return area_ctrl.calc_timespent();
}

std::vector<People_move> SmartZoneCamera::get_move() {
    return area_ctrl.calc_path();
}