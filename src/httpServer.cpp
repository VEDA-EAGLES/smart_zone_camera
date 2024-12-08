
#include "httpServer.h"

HTTPServer::HTTPServer(string ip, int port) 
{
    this->ip = ip;
    this->port = port;
}

HTTPServer::~HTTPServer() {}

void HTTPServer::setResponse()
{
    SmartZoneCamera& smartZoneCamera = SmartZoneCamera::getInstance();

    server.Post("/area/insert", [&smartZoneCamera](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();
        json jsonData = json::parse(req.body);
        jsonData = jsonData["area"][0];
        cout << jsonData["area_name"] << " is inserted in area_list" << endl;
        smartZoneCamera.area_ctrl.insertArea(
            jsonData["area_name"],
            jsonData["color"],
            jsonData["camera_id"],
            jsonData["area_id"],
            jsonData["x"],
            jsonData["y"],
            jsonData["width"],
            jsonData["height"]
        );
        json jsonHandler; jsonHandler["status"] = 200; string jsonBody = jsonHandler.dump();
        
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Delete("/area", [&smartZoneCamera](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();

        if (req.has_param("area_id")) {
            cout << "delete area_id : " << req.get_param_value("area_id") << endl;
            int areaId = stoi(req.get_param_value("area_id"));
            smartZoneCamera.area_ctrl.deleteArea(areaId);
        }
        
        json jsonHandler; jsonHandler["status"] = 200; string jsonBody = jsonHandler.dump();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});

    server.Delete("/area/all", [&smartZoneCamera](const httplib::Request& req, httplib::Response& res) {
        mtx.lock();

        cout << "delete all areas" << endl;
        smartZoneCamera.area_ctrl.deleteArea(-1);

        json jsonData; jsonData["status"] = 200; string jsonBody = jsonData.dump();
        res.set_content(jsonBody, "application/json");
        mtx.unlock();
	});
}

void HTTPServer::setHTTPServer()
{
    setResponse();
    cout << this->ip << "/" << this->port << " " << "start setHTTPServer" << endl;
    server.listen(this->ip, this->port);
}



