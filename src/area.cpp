#include "area.h"

Area::Area() {}
Area::~Area() {}

void Area::setArea(string name, int cameraId, int id, int x, int y, int width, int height)
{
    this->name = name;
    this->cameraId = cameraId;
    this->id = id;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void Area::showAreaInfo()
{
    cout << "showAreaInfo" << endl;
    cout << "name: " << name << ", " << "cameraId: " << cameraId << ", " << "id: " << id << ", " 
    << "x: " << x << ", " << "y: " << y << ", " << "width: " << width << ", " << "height: " << height << endl;
}

bool Area::deleteArea(int areaId)
{
    if (this->id == areaId) {
        return true;
    }
    else {
        return false;
    }

}

void Area_Handler::init(){
    current_time=std::chrono::steady_clock::now();
}

void Area_Handler::update(std::vector<Area> list,cv::Mat& frame) {
    area_list=list;
    prev_time=current_time;
    current_time=std::chrono::steady_clock::now();
    objects.clear();
    for (Area a:area_list) {
        int id=a.area_id;
        cv::putText(frame, cv::format("%d : %s", id, a.area_name), cv::Point(a.x, a.y - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::rectangle(frame, cv::Rect(a.x,a.y,a.width,a.height), cv::Scalar(37 * id % 255, 17 * id % 255, 29 * id % 255), 2);
    }
}

bool Area_Handler::area_within(Area a2, int id) {
    for (Area a1:area_list) {
        if (a1.x<a2.x && a1.y<a2.y && a1.x+a1.width>a2.x+a2.width/2 && a1.y+a1.height>a2.y+a2.height/2){
            if(objects[a1.area_id].find(id) == objects[a1.area_id].end()){
                objects[a1.area_id][id].Tbegin=current_time;
                objects[a1.area_id][id].Tend=current_time;
            }
            else{
                objects[a1.area_id][id].Tend=current_time;
            }
            return true;
        }
    }
    return false;
}

void Area_Handler::draw_area(cv::Mat& frame, Area a, int id) {
    int idx=id+static_cast<int>(area_list.size());
    cv::putText(frame, cv::format("%d", id), cv::Point(a.x, a.y - 5), 0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::rectangle(frame, cv::Rect(a.x,a.y,a.width,a.height), cv::Scalar(37 * idx % 255, 17 * idx % 255, 29 * idx % 255), 2);
}

void Area_Handler::calc_peoplecount(){
    std::vector<int> peoplecount;
    for (Area a1:area_list){
        int size=0;
        for (auto it = objects[a1.area_id].begin(); it != objects[a1.area_id].end(); it++) {
            if(it->second.Tend>=current_time)
                size++;
        }
        peoplecount.push_back(size);
    }
}

void Area_Handler::calc_timespent(){
    std::vector<int> timespent;
    for (Area a1:area_list){
        int size=0;
        float sum=0;
        for (auto it = objects[a1.area_id].begin(); it != objects[a1.area_id].end(); it++) {
            if(it->second.Tend>=current_time){
                size++;
                std::chrono::duration<float> duration = it->second.Tend-it->second.Tbegin;
                sum+=duration.count();
            }
        }
        if(size>0)
            timespent.push_back(sum/size);
        else
            timespent.push_back(0);
    }
}

void Area_Handler::calc_path(){//사용안함
    std::map<std::pair<int, int>, int> total_path;
    int max_id = -1;
    for (Area a1:area_list){
        int id = objects[a1.area_id].rbegin()->first;
        if(id>max_id)
            max_id=id;
    }
    for(int id=1;id<=max_id;id++){
        int prev_area_id=-1,area_id=-1;
        std::chrono::steady_clock::time_point last_time;
        for (Area a1:area_list){
            auto key=objects[a1.area_id].find(id);
            if(key!=objects[a1.area_id].end()){
                if(area_id==-1){
                    area_id=a1.area_id;
                    last_time=key->second.Tend;
                }
                else if(last_time<key->second.Tend){
                    prev_area_id=area_id;
                    area_id=a1.area_id;
                    last_time=key->second.Tend;
                }
            }
        }
        if(prev_area_id!=-1 && area_id!=-1)
            total_path[std::make_pair(prev_area_id, area_id)]++;
    }
}