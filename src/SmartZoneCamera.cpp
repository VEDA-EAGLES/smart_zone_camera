#include "SmartZoneCamera.h"
#include "area.h"
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
    cv::namedWindow("tracking", cv::WINDOW_NORMAL);

    //test용 코드
    cap.open("../model/test_video_3.mp4");

    // 비디오 파일이 열렸는지 확인
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the video file!" << std::endl;
    }

    return true;
}
std::chrono::system_clock::time_point last_video_event_time = std::chrono::system_clock::now();
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

    std::vector<Area> test;
    test.push_back(Area(0,20,600,200,1,"area1"));
    test.push_back(Area(0,320,600,200,2,"area2"));
    //test.push_back(Area(600,20,300,600,3,"area3"));
    area_ctrl.update(test,frame);

    // Draw tracking results
    for (size_t i = 0; i < output_stracks.size(); i++) {
        std::vector<float> tlwh = output_stracks[i].tlwh;
        Area a(tlwh[0],tlwh[1],tlwh[2],tlwh[3]);
        if(area_ctrl.area_within(a,output_stracks[i].track_id)){
            area_ctrl.draw_area(frame,a,output_stracks[i].track_id);
        }
    }
    area_ctrl.calc_timespent();
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = now - last_video_event_time;
    if (elapsed.count() >= ELAPSEDTIME) {
        last_video_event_time = now;
        //area_ctrl.calc_path();
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
