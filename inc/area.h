#include <opencv2/opencv.hpp>

class Area
{
private:
    int x,y,width,height;
    int area_id=0;
public:
    Area(cv::Rect rect);
    Area(cv::Rect rect, int id);
    Area(float x,float y,float width,float height);
    cv::Rect get_Area();
    int get_Area_id();
    ~Area();
};

/*class Detect_Area : public Area
{
private:
    int area_id, camera_id;
    std::string name;
public:
};*/

class Area_Controller {
private:
    std::vector<Area> area_list;
    std::map<int, std::vector<int>> peoplecount;
    //std::chrono::steady_clock::time_point Tbegin, Tend;
public:
    void update_area(std::vector<Area> list,cv::Mat& frame);
    bool area_within(Area a2, int id);
    void draw_area(cv::Mat& frame, Area a, int id);
    void calc_peoplecount(Area a, int id);
    void print_peoplecount();
};


