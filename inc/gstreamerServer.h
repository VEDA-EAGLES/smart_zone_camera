
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include "define.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include "SmartZoneCamera.h"

using namespace std;
using namespace cv;

class GstreamerServer
{
private:

public:
    GstreamerServer(/* args */);
    ~GstreamerServer();
    void setRTSPServer(int argc, char** argv);
};

