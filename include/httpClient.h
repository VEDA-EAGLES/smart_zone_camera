
#include "httplib.h"
#include "json.hpp"
#include "define.h"

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPClient
{

private:
    string getVideoInfo(string, string, string, string, string, string, string);

public:
    HTTPClient();
    ~HTTPClient();
    void startHTTPClient(httplib::Client&);
};


