#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include "define.h"
#include "httpServer.h"
#include "httpClient.h"
#include "gstreamerServer.h"
#include "area.h"

using namespace std;

void startHTTPServer(int);
void startHTTPClient(int);
void startRTSPServer(int);

mutex mtx;
vector<Area> areas;

int main(int argc, char** argv)
{
    thread httpServerThread = thread(startHTTPServer, 1);
    thread httpClientThread = thread(startHTTPClient, 2);
    
    GstreamerServer rtspServer;
    rtspServer.setRTSPServer(argc,argv);

    httpServerThread.join();
    httpClientThread.join();

    return 0;
}

void startHTTPServer(int thread_id) 
{
    HTTPServer httpServer(MY_IP, HTTP_S_PORT);
    httpServer.setHTTPServer();
}

void startHTTPClient(int thread_id) 
{
    HTTPClient httpClient;
    httplib::Client cli(SERVER_IP, HTTP_C_PORT);

    while(true) {
        httpClient.startHTTPClient(cli);
        this_thread::sleep_for(std::chrono::minutes(1));
    }
}

