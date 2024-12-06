
#include "gstreamerServer.h"

GstreamerServer::GstreamerServer()
{
}

GstreamerServer::~GstreamerServer()
{
}

static gboolean serverStartedHandler(gpointer user_data) {
    // 서버 주소 출력
    g_print("rtsp server ready : rtsp://192.168.0.107:8082/test\n");

    // 핸들러가 한 번만 실행되도록 TRUE 반환
    return G_SOURCE_REMOVE; 
}

void prcessFrame(int handler) 
{
    // 비디오 캡처 설정 (카메라에서 영상 캡처)
    cout << "prcessFrame" << endl;
    cv::VideoCapture cap("libcamerasrc ! videoconvert ! video/x-raw, framerate=30/1, width=640, height=480, format=BGR ! appsink", cv::CAP_GSTREAMER);

    if (!cap.isOpened()) {
        cout << "can't create video capture\n";
        return;
    }

    // 비디오 writer 설정 (H.264 인코딩, UDP 스트리밍)
    cv::VideoWriter writer;
    writer.open("appsrc ! videoconvert ! video/x-raw, format=I420 ! x264enc tune=zerolatency ! rtph264pay ! udpsink host=192.168.0.107 port=8554",
                cv::CAP_GSTREAMER, 0, 30.0, cv::Size(640, 480), true);

    if (!writer.isOpened()) {
        cout << "can't create video writer\n";
        return;
    }

    cv::Mat frame;

    while (true) {
        cap >> frame;  // 프레임을 캡처합니다.
        if (frame.empty())
            break;

        // 비디오에 텍스트 추가
        cv::putText(frame, "test", Point(10, 40), 2, 1.2, Scalar(0, 255, 0));

        // 비디오 스트리밍을 위해 writer에 프레임을 기록합니다.
        writer.write(frame);
    }

}

void GstreamerServer::setRTSPServer(int argc, char** argv)
{
    thread opencvProcessing = thread(prcessFrame, NULL);

    gst_init(&argc, &argv);
    GstRTSPServer *server = gst_rtsp_server_new();
    g_object_set(server, "service", RTSP_S_PORT, NULL);

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);

    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    GMainLoop *loop = g_main_loop_new(NULL, false);

    gst_rtsp_media_factory_set_launch(factory, "(udpsrc port=8554 caps=\"application/x-rtp,clock-rate=90000,payload=96\" ! rtph264depay ! avdec_h264 ! videoconvert ! x264enc ! rtph264pay name=pay0 pt=96 )");
    
    gst_rtsp_mount_points_add_factory (mounts, "/test", factory);

    g_object_unref(mounts);

    gst_rtsp_server_attach(server, NULL); 

    g_idle_add(serverStartedHandler, NULL); 
    g_main_loop_run (loop);
    
    // Clean up
    g_main_loop_unref(loop);
    gst_object_unref(server);
    opencvProcessing.detach();

}
