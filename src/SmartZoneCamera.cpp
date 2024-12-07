#include "SmartZoneCamera.h"
#include "area.h"
#include <iostream>

SmartZoneCamera::SmartZoneCamera()
    : fps(15), tracker(fps, 15), frame(640, 480, CV_8UC3, cv::Scalar(255)) {}

SmartZoneCamera::~SmartZoneCamera() {
    finalize(); // Ensure resources are released
}

bool SmartZoneCamera::initialize() {
    std::cout << "Initializing SmartZoneCamera..." << std::endl;

    detector.init("../model/yoloxN.param", "../model/yoloxN.bin");

    std::string pipeline = 
        "libcamerasrc ! videoconvert ! video/x-raw, framerate=10/1, width=640, height=480, format=BGR ! appsink";

    cap.open(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened()) {
        cout << "can't create video capture\n";
        return false;
    }
    // cv::VideoWriter writer;
    udpWriter.open("appsrc ! videoconvert ! video/x-raw, format=I420 ! x264enc tune=zerolatency ! rtph264pay ! udpsink host=112.186.2.175 port=8555",
        cv::CAP_GSTREAMER, 0, 30.0, cv::Size(640, 480), true);

    if (!udpWriter.isOpened()) {
        cout << "can't create video writer\n";
        return false;
    }

    return true;
}

void SmartZoneCamera::processFrame() {
    /*if (!cam.getVideoFrame(frame, 1000)) {
        std::cerr << "Timeout error while grabbing frame." << std::endl;
        return;
    }*/
    cap >> frame; // 또는 cap.read(frame);

    // 프레임이 없으면 (영상이 끝났으면) 종료
    if (frame.empty()) {
        std::cout << "End of video." << std::endl;
    }
    fpsInfo.startCheckFrame();
    // Object detection
    std::vector<Object> objects;
    detector.detect_yolox(frame, objects);
    // Object tracking
    std::vector<STrack> output_stracks = tracker.update(objects);
    
    area_ctrl.update(frame);
    // Draw tracking results
    for (size_t i = 0; i < output_stracks.size(); i++) {
        std::vector<float> tlwh = output_stracks[i].tlwh;
        Area a(tlwh[0],tlwh[1],tlwh[2],tlwh[3]);
        if(area_ctrl.area_within(a,output_stracks[i].track_id)){
            area_ctrl.draw_area(frame,a,output_stracks[i].track_id);
        }
    }
    // area_ctrl.calc_path();
    fpsInfo.endCheckFrame();
    float FPS = fpsInfo.calculateFrame();
    cv::putText(frame, cv::format("FPS %0.2f", FPS / 16), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255));

    // cv::imshow("tracking", frame);
    // Save original frame
    // if (!originalVideoWriter.isOpened()) {
    //     originalVideoWriter.open("./original_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1280, 960), true);
    // }
    // originalVideoWriter.write(frame);

    // Save tracked frame
    // if (!trackingVideoWriter.isOpened()) {
    //     trackingVideoWriter.open("./tracking_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(1280, 960), true);
    // }
    // trackingVideoWriter.write(frame);
    // cv::putText(frame, "test", Point(10, 40), 2, 1.2, Scalar(0, 255, 0));
    udpWriter.write(frame);
}

void SmartZoneCamera::run() {
    std::cout << "Starting main loop. Press ESC to exit." << std::endl;
    
    while (true) {
        processFrame();

        // Exit on ESC key
        // char esc = cv::waitKey(5);
        // if (esc == 27) break;
    }
}

void SmartZoneCamera::finalize() {
    std::cout << "Releasing resources..." << std::endl;
    cam.stopVideo();
    cv::destroyAllWindows();
    originalVideoWriter.release();
    trackingVideoWriter.release();
}
