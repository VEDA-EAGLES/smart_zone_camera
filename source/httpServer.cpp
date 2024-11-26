
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
        cout << req.body << endl;
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



