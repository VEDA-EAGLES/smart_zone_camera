
#include "httpServer.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

void HTTPServer::setResponse()
{
    server.Post("/area/insert", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        jsonData = jsonData["area"][0];
        Area area;
        area.setArea(jsonData["area_name"],jsonData["camera_id"],jsonData["area_id"],jsonData["x"],jsonData["y"],jsonData["width"],jsonData["height"]);
        areas.push_back(area);
        
        for (auto x : areas) {x.showAreaInfo();}
        
        json jsonHandler; jsonHandler["status"] = 200; string jsonBody = jsonHandler.dump();
        
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Delete("/area/delete", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        for (auto it = areas.begin(); it != areas.end(); it++) {  
            if (it->deleteArea(jsonData["area_id"])) {
                it = areas.erase(it);  
                break;
            }
        }
        for (auto x : areas) {x.showAreaInfo();}
        json jsonHandler; jsonHandler["status"] = 200; string jsonBody = jsonHandler.dump();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Delete("/area/all", [](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        areas.clear();
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



