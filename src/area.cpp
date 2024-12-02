
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