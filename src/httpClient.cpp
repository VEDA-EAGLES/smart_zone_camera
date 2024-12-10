
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
        json arr;
        arr["data"] = json::array();
        for (auto p:peopleCnts) {
            json data;
            data["area_id"] = p.area_id;
            data["people_count"] = p.count;
            data["start_time"] = p.start_time;
            data["end_time"] = p.end_time;
            arr["data"].push_back(data);
        }
        string jsonBody = arr.dump();
        auto res = cli.Post("/peoplecnt", jsonBody, "application/json");
        if (!res) {  
            std::cout << "Error: " << res.error() << std::endl;
        } 
    }
    mtx.unlock();
}
// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::sendPeopleMove(httplib::Client& cli, std::vector<People_move> peopleMoves)
{  
    mtx.lock();
    if (!peopleMoves.empty()) {
        std::cout << " sendPeopleMove " << endl;

        json arr;
        arr["data"] = json::array();
        for (auto p:peopleMoves) {
            json data;
            data["from_area_id"] = p.from_area_id;
            data["to_area_id"] = p.to_area_id;
            data["count"] = p.count;
            data["start_time"] = p.start_time;
            data["end_time"] = p.end_time;
            arr["data"].push_back(data);
        }
        string jsonBody = arr.dump();
        auto res = cli.Post("/peoplemove", jsonBody, "application/json");
        if (!res) {  
            std::cout << "Error: " << res.error() << std::endl;
        } 
    }
    mtx.unlock();
}
// 카메라에서 서버로 주기적 비디오 정보 송신
void HTTPClient::sendPeopleStay(httplib::Client& cli, std::vector<People_stay> peopleStays)
{  
    mtx.lock();
    if (!peopleStays.empty()) {
        std::cout << " sendPeopleStay " << endl;

        json arr;
        arr["data"] = json::array();
        for (auto p:peopleStays) {
            json data;
            data["area_id"] = p.area_id;
            data["stay_time"] = p.stay_time;
            data["start_time"] = p.start_time;
            data["end_time"] = p.end_time;
            arr["data"].push_back(data);
        }
        string jsonBody = arr.dump();
        auto res = cli.Post("/peoplestay", jsonBody, "application/json");
        if (!res) {  
            std::cout << "Error: " << res.error() << std::endl;
        } 
    }
    mtx.unlock();
}
