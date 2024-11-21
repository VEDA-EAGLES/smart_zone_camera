
#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

string HTTPClient::getVideoInfo(string areaId, string cameraId, string x1, string y1, string x2, string y2, string areaName)
{
    // JSON 데이터 생성
    json jsonData;
    jsonData["area"] = json::array();

    json areaData;
    areaData["area_id"] = areaId;
    areaData["camera_id"] = cameraId;
    areaData["top_left"]["x"] = x1;
    areaData["top_left"]["y"] = y1;
    areaData["bottom_right"]["x"] = x2;
    areaData["bottom_right"]["y"] = y2;
    areaData["area_name"] = areaName;

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
