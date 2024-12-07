#pragma once
#include <opencv2/opencv.hpp>

struct Area
{
    int x,y,width,height;
    int area_id;
    std::string area_name;
    Area(int x, int y, int width, int height, int area_id=0, std::string area_name=""){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->area_id = area_id;
        this->area_name = area_name;
    };
    Area(float x, float y, float width, float height){
        this->x = static_cast<int>(x);
        this->y = static_cast<int>(y);
        this->width = static_cast<int>(width);
        this->height = static_cast<int>(height);
    };
};

struct ObjectInfo{
    std::chrono::steady_clock::time_point Tbegin, Tend;
};

class Area_Handler {
private:
    std::vector<Area> area_list;
    std::map<int, std::map<int, ObjectInfo>> objects; //key: area_id, value:(key:object_id, value:Tbegin, Tend)
    std::chrono::steady_clock::time_point current_time, prev_time;
public:
    void init();
    void update(std::vector<Area> list,cv::Mat& frame);
    bool area_within(Area a2, int id);
    void draw_area(cv::Mat& frame, Area a, int id);
    void calc_peoplecount();// 영역 내 사람 수
    void calc_timespent();//영역 내 머문 시간의 평균
    void calc_path();//영역 간 이동한 사람 수
};


