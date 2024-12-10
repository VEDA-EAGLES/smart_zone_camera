
#include "SmartZoneCamera.h"
#include "httplib.h"
#include "json.hpp"
#include "define.h"
#include <vector>

using namespace nlohmann;
using namespace std;

extern mutex mtx;

class HTTPClient
{

private:

public:
    HTTPClient();
    ~HTTPClient();
    httplib::Client setClient(string clientIp, int port);
    void sendPeopleCount(httplib::Client& cli, std::vector<People_count> peopleCnts);
    void sendPeopleMove(httplib::Client& cli, std::vector<People_move> peopleMoves);
    void sendPeopleStay(httplib::Client& cli, std::vector<People_stay> peopleStays);
};


