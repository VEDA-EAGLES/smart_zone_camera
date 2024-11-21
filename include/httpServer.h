
#include "httplib.h"
#include "json.hpp"
#include "define.h"

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPServer
{
private:
    httplib::Server server;
    string ip;
    int port;
    void setResponse();

public:
    HTTPServer(string ip, int port);
    ~HTTPServer();
    void setHTTPServer();
};


