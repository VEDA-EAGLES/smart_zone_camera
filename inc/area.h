#ifndef AREA_H // AREA_H는 헤더 파일 이름을 대문자로 바꾸고 .h를 _H로 바꾼 것입니다.
#define AREA_H

#include <vector>
#include <iostream>
#include <mutex>

using namespace std;

extern mutex mtx;

class Area
{
private:
    int cameraId;
    int id;
    string name;
    int x,y,width,height;

public:
    Area();
    ~Area();
    void setArea(string name, int cameraId, int id, int x, int y, int width, int height);
    void showAreaInfo();
    bool deleteArea(int areaId);
};


#endif // AREA_H