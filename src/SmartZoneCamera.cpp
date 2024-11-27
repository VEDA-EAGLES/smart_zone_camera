#include "SmartZoneCamera.h"
#include <iostream>

SmartZoneCamera::SmartZoneCamera()
    : fps(30), tracker(fps, 30), frame(1280, 960, CV_8UC3, cv::Scalar(255)) {}

SmartZoneCamera::~SmartZoneCamera() {
    finalize(); // Ensure resources are released
}

bool SmartZoneCamera::initialize() {
    std::cout << "Initializing SmartZoneCamera..." << std::endl;

    cam.options->video_width = 1280;
    cam.options->video_height = 960;
    cam.options->framerate = fps;
    cam.options->verbose = true;

    if (!cam.startVideo()) {
        std::cerr << "Failed to start video capture." << std::endl;
        return false;
    }

    detector.init("../model/yoloxN.param", "../model/yoloxN.bin");
    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    return true;
}

void SmartZoneCamera::processFrame() {
    if (!cam.getVideoFrame(frame, 1000)) {
        std::cerr << "Timeout error while grabbing frame." << std::endl;
        return;
    }

    fpsInfo.startCheckFrame();

    // Object detection
    std::vector<Object> objects;
    detector.detect_yolox(frame, objects);

    // Object tracking
    std::vector<STrack> output_stracks = tracker.update(objects);

    // Draw tracking results
    for (size_t i = 0; i < output_stracks.size(); i++) {
        std::vector<float> tlwh = output_stracks[i].tlwh;
        cv::Scalar s = tracker.get_color(output_stracks[i].track_id);
        cv::putText(frame, cv::format("%d", output_stracks[i].track_id), cv::Point(tlwh[0], tlwh[1] - 5),
                    0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        cv::rectangle(frame, cv::Rect(tlwh[0], tlwh[1], tlwh[2], tlwh[3]), s, 2);
    }

    fpsInfo.endCheckFrame();
    float FPS = fpsInfo.calculateFrame();
    cv::putText(frame, cv::format("FPS %0.2f", FPS / 16), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255));

    cv::imshow("tracking", frame);

    // Save original frame
    if (!originalVideoWriter.isOpened()) {
        originalVideoWriter.open("./original_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1280, 960), true);
    }
    originalVideoWriter.write(frame);

    // Save tracked frame
    if (!trackingVideoWriter.isOpened()) {
        trackingVideoWriter.open("./tracking_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1280, 960), true);
    }
    trackingVideoWriter.write(frame);
}

void SmartZoneCamera::run() {
    std::cout << "Starting main loop. Press ESC to exit." << std::endl;
    while (true) {
        processFrame();

        // Exit on ESC key
        char esc = cv::waitKey(5);
        if (esc == 27) break;
    }
}

void SmartZoneCamera::finalize() {
    std::cout << "Releasing resources..." << std::endl;
    cam.stopVideo();
    cv::destroyAllWindows();
    originalVideoWriter.release();
    trackingVideoWriter.release();
}
