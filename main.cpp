
#include <iostream>
#include <thread>
#include <mutex>
#include "define.h"
#include "httpServer.h"
#include "httpClient.h"
#include "gstreamerServer.h"
#include "SmartZoneCamera.h"

using namespace std;

bool camInit(string myIp,cv::VideoCapture& cap, cv::VideoWriter& udpWriter);

mutex mtx;
string MY_IP;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Ip Address>" << std::endl;
        return 1;
    }
    MY_IP = argv[1];
    std::cout << "IP ADDRESS: " << MY_IP << std::endl;

    SmartZoneCamera& smartZoneCamera = SmartZoneCamera::getInstance();
    smartZoneCamera.init();
    
    thread receiveHTTP([]() {
        HTTPServer httpServer(MY_IP, HTTP_S_PORT);
        httpServer.setHTTPServer();
    });

    thread sendHTTP([&smartZoneCamera]() {
        HTTPClient httpClient;
        httplib::Client cli = httpClient.setClient(SERVER_IP,HTTP_C_PORT);
        while(true) {
            // this_thread::sleep_for(std::chrono::minutes(1));
            this_thread::sleep_for(std::chrono::seconds(10));
            httpClient.sendPeopleCount(cli,smartZoneCamera.get_peoplecount());
            httpClient.sendPeopleMove(cli,smartZoneCamera.get_move());
            httpClient.sendPeopleStay(cli,smartZoneCamera.get_stay());
        }
    });
    
    thread operateProcess([&smartZoneCamera]() {
        cv::VideoCapture cap;
        cv::VideoWriter udpWriter;
        cv::Mat frame(640, 480, CV_8UC3, cv::Scalar(255));
        camInit(MY_IP,cap,udpWriter);
        while(true) {
            cap >> frame; 
            // 프레임이 없으면 (영상이 끝났으면) 종료
            if (frame.empty()) {
                std::cout << "End of video." << std::endl;
            }
            udpWriter.write(smartZoneCamera.processFrame(frame));
        }
    });
    
    GstreamerServer rtspServer;
    rtspServer.setRTSPServer(MY_IP);

    receiveHTTP.detach();
    sendHTTP.detach();
    operateProcess.detach();

    return 0;
}

bool camInit(string myIp,cv::VideoCapture& cap, cv::VideoWriter& udpWriter) 
{
    std::cout << "Initializing Camera..." << std::endl;

    // Open the camera with GStreamer pipeline
    std::string capturePipeline = "libcamerasrc ! videoconvert ! video/x-raw,framerate=10/1,width=640,height=480,format=BGR ! queue ! appsink";

    cap.open(capturePipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open video capture pipeline." << std::endl;
        return false;
    }

    std::string writerPipeline = "appsrc is-live=true ! videoconvert ! video/x-raw,format=I420 ! queue ! x264enc ! rtph264pay ! udpsink host=" + myIp +" port=5000";
    udpWriter.open(writerPipeline,cv::CAP_GSTREAMER, 0, 10.0, cv::Size(640, 480), true);
    
    if (!udpWriter.isOpened()) {
        cout << "can't create video writer\n";
        return false;
    }

    return true;
}


