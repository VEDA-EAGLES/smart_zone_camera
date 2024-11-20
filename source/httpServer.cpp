
#include "httpServer.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

string HTTPServer::getVideoInfo(string areaId, string cameraId, string x1, string y1, string x2, string y2, string areaName)
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

void HTTPServer::setResponse()
{
    server.Get("/video", [this](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        string jsonBody = getVideoInfo("areaId","cameraId","x1","y1","x2","y2","areaName");
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/area/insert", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData; jsonData["status"] = 200; string jsonBody = jsonData.dump();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Get("/area/delete", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData; jsonData["status"] = 200; string jsonBody = jsonData.dump();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});
}

void HTTPServer::setHTTPServer()
{
    setResponse();
    server.listen(this->ip, this->port);
}



