#pragma once
#include <opencv2/opencv.hpp>

#define ELAPSEDTIME 60.0

struct Area
{
    int x,y,width,height;
    int areaId,cameraId;
    std::string areaName,color;
    Area(int x, int y, int width, int height, int areaId=0, int cameraId=0,std::string areaName="",std::string color=""){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->areaId = areaId;
        this->cameraId = cameraId;
        this->areaName = areaName;
        this->color = color;
    };
    Area(float x, float y, float width, float height){
        this->x = static_cast<int>(x);
        this->y = static_cast<int>(y);
        this->width = static_cast<int>(width);
        this->height = static_cast<int>(height);
    };
};

struct People_count {
    int area_id, count;
    int start_time, end_time;
    People_count(int area_id, int count, std::chrono::system_clock::time_point start_time, std::chrono::system_clock::time_point end_time) {
        this->area_id=area_id;
        this->count=count;
        this->start_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(start_time.time_since_epoch()).count());
        this->end_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(end_time.time_since_epoch()).count());
    };
};

struct People_stay {
    int area_id, stay_time;
    int start_time, end_time;
    People_stay(int area_id, int stay_time, std::chrono::system_clock::time_point start_time, std::chrono::system_clock::time_point end_time) {
        this->area_id=area_id;
        this->stay_time=stay_time;
        this->start_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(start_time.time_since_epoch()).count());
        this->end_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(end_time.time_since_epoch()).count());
    };
};

struct People_move {
    int from_area_id, to_area_id, count;
    int start_time, end_time;
    People_move(int from_area_id,int to_area_id, int count, std::chrono::system_clock::time_point start_time, std::chrono::system_clock::time_point end_time) {
        this->from_area_id=from_area_id;
        this->to_area_id=to_area_id;
        this->count=count;
        this->start_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(start_time.time_since_epoch()).count());
        this->end_time=static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(end_time.time_since_epoch()).count());
    };
};

struct ObjectInfo{
    std::chrono::system_clock::time_point Tbegin, Tend;
};

class Area_Handler {
private:
    std::vector<Area> area_list;
    std::map<int, std::map<int, ObjectInfo>> objects; //key: area_id, value:(key:object_id, value:Tbegin, Tend)
    std::chrono::system_clock::time_point current_time, prev_time;
    std::vector<int> lost_objects;
public:
    void insertArea(std::string name, std::string color, int cameraId, int id, int x, int y, int width, int height);
    void deleteArea(int areaId);    // areaId = -1 -> deleteAll else delete areaId
    void init();
    void update(cv::Mat& frame);
    bool area_within(Area a2, int id);
    void draw_area(cv::Mat& frame, Area a, int id);
    std::vector<People_count> calc_peoplecount();// 영역 내 사람 수
    std::vector<People_stay> calc_timespent();//영역 내 사람이 사라지면 머문시간 반환
    std::vector<People_move> calc_path();//영역 간 이동한 사람 수
};


