
#include "httplib.h"
#include "define.h"

using namespace std;

extern mutex mtx;

class HTTPClient
{

private:

public:
    HTTPClient();
    ~HTTPClient();
    void startHTTPClient(httplib::Client&);
};


