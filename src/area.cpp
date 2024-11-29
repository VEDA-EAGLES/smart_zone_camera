
#include "area.h"

Area::Area(cv::Rect rect)
{
    x=rect.x;
    y=rect.y;
    width=rect.width;
    height=rect.height;
}

Area::Area(cv::Rect rect, int id){
    x=rect.x;
    y=rect.y;
    width=rect.width;
    height=rect.height;
    area_id=id;
}

Area::Area(float x,float y,float width,float height)
{
    this->x=static_cast<int>(x);
    this->y=static_cast<int>(y);
    this->width=static_cast<int>(width);
    this->height=static_cast<int>(height);
}

cv::Rect Area::get_Area() {
    return cv::Rect(x,y,width,height);
}

int Area::get_Area_id(){
    return area_id;
}

Area::~Area()
{
}

void Area_Controller::update_area(std::vector<Area> list,cv::Mat& frame) {
    area_list=list;
    int id=1;
    for (Area a:area_list) {
        cv::putText(frame, cv::format("%d", a.get_Area_id()), cv::Point(a.get_Area().x, a.get_Area().y - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::rectangle(frame, a.get_Area(), cv::Scalar(37 * id % 255, 17 * id % 255, 29 * id % 255), 2);
        id+=1;
    }
}

bool Area_Controller::area_within(Area a2, int id) {
    for (Area a1:area_list) {
        if (a1.get_Area().x<a2.get_Area().x && a1.get_Area().y<a2.get_Area().y
            && a1.get_Area().x+a1.get_Area().width>a2.get_Area().x+a2.get_Area().width/2
            && a1.get_Area().y+a1.get_Area().height>a2.get_Area().y+a2.get_Area().height/2){
            calc_peoplecount(a1,id);
            return true;
        }
    }
    return false;
}

void Area_Controller::draw_area(cv::Mat& frame, Area a, int id) {
    int idx=id+static_cast<int>(area_list.size());
    cv::putText(frame, cv::format("%d", id), cv::Point(a.get_Area().x, a.get_Area().y - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    cv::rectangle(frame, a.get_Area(), cv::Scalar(37 * idx % 255, 17 * idx % 255, 29 * idx % 255), 2);
}

void Area_Controller::calc_peoplecount(Area a, int id){
    peoplecount[a.get_Area_id()].push_back(id);
}

void Area_Controller::print_peoplecount(){
    for(Area a:area_list){
        std::cout<<a.get_Area_id()<<" : "<<peoplecount[a.get_Area_id()].size()<<std::endl;
    }
}