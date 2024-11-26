
#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

string HTTPClient::getVideoInfo(string areaName, int areaId, int cameraId, int x, int y, int width, int height)
{
    // JSON 데이터 생성
    json jsonData;
    jsonData["area"] = json::array();

    json areaData;
    areaData["area_name"] = areaName;
    areaData["area_id"] = areaId;
    areaData["camera_id"] = cameraId;
    areaData["x"] = x;
    areaData["y"] = y;
    areaData["width"] = width;
    areaData["height"] = height;

    jsonData["area"].push_back(areaData);

    // JSON 데이터를 문자열로 변환
    string jsonBody = jsonData.dump();
    return jsonBody;                     
}

void HTTPClient::startHTTPClient(httplib::Client& cli)
{  
    mtx.lock();
    string jsonBody = getVideoInfo("areaId","cameraId","x1","y1","x2","y2","areaName");
    auto res = cli.Post("/video", jsonBody, "application/json");
    if (res) {  
    } else {
        std::cout << "Error: " << res.error() << std::endl;
    }
    mtx.unlock();
}
