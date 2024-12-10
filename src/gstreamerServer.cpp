#include "gstreamerServer.h"

GstreamerServer::GstreamerServer()
{
}

GstreamerServer::~GstreamerServer()
{
}

static gboolean serverStartedHandler(gpointer user_data) {
    // 핸들러가 한 번만 실행되도록 TRUE 반환
    return G_SOURCE_REMOVE; 
}

void GstreamerServer::setRTSPServer(string myIp)
{
    // Initialize GStreamer
    gst_init(nullptr, nullptr);

    // Create the RTSP server
    GstRTSPServer *server = gst_rtsp_server_new();
    g_object_set(server, "service", RTSP_S_PORT, NULL);

    GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points(server);

    // Create a media factory
    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();
    gst_rtsp_media_factory_set_launch(
        factory,
        "( udpsrc port=5000 caps=\"application/x-rtp,clock-rate=90000,payload=96\" ! rtph264depay ! h264parse ! rtph264pay name=pay0 pt=96 )"
    );
    gst_rtsp_media_factory_set_shared(factory, TRUE);

    // Attach the factory to the RTSP server
    gst_rtsp_mount_points_add_factory(mounts, "/test", factory);
    g_object_unref(mounts);

    // Attach the server to the default main context
    gst_rtsp_server_attach(server, NULL);

    std::cout << "RTSP server is running at rtsp://" + myIp + ":" + RTSP_S_PORT + "/test" << std::endl;

    // Run the main loop
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    // Clean up
    g_main_loop_unref(loop);
    gst_object_unref(server);

}