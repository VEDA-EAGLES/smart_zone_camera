
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
    // // 싱글톤 인스턴스 가져오기
    SmartZoneCamera& smartZoneCamera = SmartZoneCamera::getInstance();

    if (!smartZoneCamera.initialize()) {
        std::cerr << "Failed to initialize the camera system." << std::endl;
        return;
    }
    smartZoneCamera.run();
}

void GstreamerServer::setRTSPServer(int argc, char** argv)
{
    thread opencvProcessing = thread(prcessFrame, NULL);

    g_usleep(2000000);  // 2000000 마이크로초 = 2초

    gst_init(&argc, &argv);
    GstRTSPServer *server = gst_rtsp_server_new();
    g_object_set(server, "service", RTSP_S_PORT, NULL);

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);

    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    GMainLoop *loop = g_main_loop_new(NULL, false);

    gst_rtsp_media_factory_set_launch(factory, "(udpsrc port=8083 caps=\"application/x-rtp,clock-rate=90000,payload=96\" ! rtph264depay ! queue ! avdec_h264 ! videoconvert ! x264enc ! rtph264pay name=pay0 pt=96 )");

    gst_rtsp_mount_points_add_factory (mounts, "/test", factory);   

    gst_rtsp_media_factory_set_shared(factory, TRUE);

    g_object_unref(mounts);
    
    gst_rtsp_server_attach(server, NULL); 

    g_idle_add(serverStartedHandler, NULL); 

    g_main_loop_run (loop);
    
    // Clean up
    opencvProcessing.detach();
    g_main_loop_unref(loop);
    gst_object_unref(server);

}
