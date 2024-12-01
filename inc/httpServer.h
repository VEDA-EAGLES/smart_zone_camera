
#include "httplib.h"
#include "json.hpp"
#include "define.h"
#include "area.h"
#include <vector>

using namespace nlohmann;
using namespace std;

extern mutex mtx;
extern vector<Area> areas;

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


