
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include "define.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include "SmartZoneCamera.h"
#include <string>

using namespace std;
using namespace cv;

class GstreamerServer
{
private:

public:
    GstreamerServer(/* args */);
    ~GstreamerServer();
    void setRTSPServer(string myIp);
};

