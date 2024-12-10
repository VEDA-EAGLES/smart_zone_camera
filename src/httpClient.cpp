
#include "httpClient.h"

HTTPClient::HTTPClient() {}
HTTPClient::~HTTPClient() {}

// 주기적으로 송신할 주소 설정
httplib::Client HTTPClient::setClient(string clientIp, int port)
{
    std::cout << "Send Video-Info To "<< clientIp << " Periodically" << endl;
    httplib::Client cli(SERVER_IP, HTTP_C_PORT);
    return cli;
}

// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::sendPeopleCount(httplib::Client& cli, std::vector<People_count> peopleCnts) 
{  
    mtx.lock();
    if (!peopleCnts.empty()) {
         std::cout << " sendPeopleCount " << endl;

        


    }
    mtx.unlock();
}
// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::sendPeopleMove(httplib::Client& cli, std::vector<People_move> peopleMoves)
{  
    mtx.lock();
    if (!peopleMoves.empty()) {
         std::cout << " sendPeopleMove " << endl;
    }
    mtx.unlock();
}
// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::sendPeopleStay(httplib::Client& cli, std::vector<People_stay> peopleStays)
{  
    mtx.lock();
    if (!peopleStays.empty()) {
         std::cout << " sendPeopleStay " << endl;
    }
    mtx.unlock();
}
