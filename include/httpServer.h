
#include "httplib.h"
#include "json.hpp"
#include "define.h"

using namespace std;
using namespace nlohmann;

extern mutex mtx;

class HTTPServer
{
private:
    httplib::Server server;
    string ip;
    int port;
    void setResponse();
    string getVideoInfo(string, string, string, string, string, string, string);

public:
    HTTPServer(string ip, int port);
    ~HTTPServer();
    void setHTTPServer();
};


