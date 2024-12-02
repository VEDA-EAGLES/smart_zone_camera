
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

// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::startHTTPClient(httplib::Client& cli)
{  
    mtx.lock();
    string jsonBody = getVideoInfo("areaName",1,0,0,0,0,0);
    auto res = cli.Post("/video", jsonBody, "application/json");
    if (res) {  
    } else {
        std::cout << "Error: " << res.error() << std::endl;
    }
    mtx.unlock();
}
