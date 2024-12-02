
#include "gstreamerServer.h"

GstreamerServer::GstreamerServer()
{
}

GstreamerServer::~GstreamerServer()
{
}

static void client_connected(GstRTSPServer * server, GstRTSPClient * client, gpointer user_data)
{
  g_print ("Client connected\n");
}

void GstreamerServer::setRTSPServer()
{
    GstRTSPServer *server = gst_rtsp_server_new();
    g_object_set(server, "service", RTSP_S_PORT, NULL);

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);

    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();

    GMainLoop *loop = g_main_loop_new(NULL, false);

    gst_rtsp_media_factory_set_launch(factory, "(libcamerasrc ! video/x-raw,format=YUY2,width=1920,height=1080,framerate=15/1 ! videoconvert ! video/x-raw,format=I420 ! x264enc ! rtph264pay name=pay0 pt=96 )");
    
    gst_rtsp_mount_points_add_factory (mounts, "/test", factory);

    g_object_unref(mounts);

    gst_rtsp_server_attach(server, NULL); 

    // 서버 주소 출력
    g_print("rtsp server ready : rtsp://192.168.0.107:8082/test\n");

    g_main_loop_run (loop);
    
    // Clean up
    g_main_loop_unref(loop);
    gst_object_unref(server);

    


}
