
#include "gstreamerServer.h"

GstreamerServer::GstreamerServer()
{
}

GstreamerServer::~GstreamerServer()
{
}

// static gboolean client_connected (GstRTSPServer * server, GstRTSPClient * client, gpointer user_data)
// {
//   g_print ("Client connected\n");
//   return TRUE;
// }

// void GstreamerServer::setRTSPServer()
// {
//     GMainLoop *loop;
//     GstRTSPServer *server;
//     GstRTSPMountPoints *mounts;
//     GstRTSPMediaFactory *factory;

//     loop = g_main_loop_new(NULL, false);

//     server = gst_rtsp_server_new();
//     g_signal_connect(server, "client-connected", G_CALLBACK(client_connected), NULL);

//     mounts = gst_rtsp_server_get_mount_points(server);
//     factory = gst_rtsp_media_factory_new();
//     gst_rtsp_media_factory_set_launch (factory,"(rtspsrc location=rtsp://192.168.0.107:8002/test_stream ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! x264enc ! rtph264pay name=pay0 pt=96 )");
//     gst_rtsp_media_factory_set_shared (factory, true);
//     gst_rtsp_mount_points_add_factory (mounts, "/test", factory);
//     g_object_unref(mounts);

//     gst_rtsp_server_attach(server, NULL);

//     g_print("stream ready at rtsp://127.0.0.1:8554/test\n");
//     g_main_loop_run (loop);

// }

