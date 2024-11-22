
#include "area.h"

area::area(cv::Rect rect)
{
    x=rect.x;
    y=rect.y;
    width=rect.width;
    height=rect.height;
}

area::area(cv::Rect_<float> rect)
{
    x=static_cast<int>(rect.x);
    y=static_cast<int>(rect.y);
    width=static_cast<int>(rect.width);
    height=static_cast<int>(rect.height);
}

area::~area()
{
}