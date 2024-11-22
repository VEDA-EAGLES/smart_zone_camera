#include <opencv2/opencv.hpp>
class area

{
private:
    int x,y,width,height;
public:
    area(cv::Rect rect);
    area(cv::Rect_<float> rect);
    ~area();
};


