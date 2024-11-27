
#include "httplib.h"
#include "json.hpp"
#include "define.h"

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPClient
{

private:
    string getVideoInfo(string areaName, int areaId, int cameraId, int x, int y, int width, int height);

public:
    HTTPClient();
    ~HTTPClient();
    void startHTTPClient(httplib::Client&);
};


