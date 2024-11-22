#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include "yoloX.h"  // yolox 관련 헤더 파일

int main(int argc, char** argv)
{
    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop." << std::endl;
    cv::Mat frame(1280, 960, CV_8UC3, cv::Scalar(255));
    lccv::PiCamera cam;
    cam.options->video_width = 1280;
    cam.options->video_height = 960;
    cam.options->framerate = 15;
    cam.options->verbose = true;
    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    cam.startVideo();
    float f;
    float FPS[16];
    int i, Fcnt = 0;
    std::chrono::steady_clock::time_point Tbegin, Tend;

    for(i = 0; i < 16; i++) FPS[i] = 0.0;

    // YOLOX 모델 초기화
    YoloX detector;
    detector.init("../model/yoloxN.param", "../model/yoloxN.bin");

    int fps = cam.options->framerate;

    BYTETracker tracker(fps, 30);

    std::cout << "Start grabbing, press ESC on Live window to terminate" << std::endl;

    while(1) {
        if (!cam.getVideoFrame(frame, 1000)) {
            std::cout << "Timeout error" << std::endl;
        }

        Tbegin = std::chrono::steady_clock::now();

        std::vector<Object> objects;
        detector.detect_yolox(frame, objects);  // 객체 탐지

        // 트래킹 처리
        std::vector<STrack> output_stracks = tracker.update(objects);

        for (size_t i = 0; i < output_stracks.size(); i++) {
            std::vector<float> tlwh = output_stracks[i].tlwh;
            cv::Scalar s = tracker.get_color(output_stracks[i].track_id);
            cv::putText(frame, cv::format("%d" , output_stracks[i].track_id), cv::Point(tlwh[0], tlwh[1] - 5),
                        0, 0.6, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
            cv::rectangle(frame, cv::Rect(tlwh[0], tlwh[1], tlwh[2], tlwh[3]), s, 2);
        }

        Tend = std::chrono::steady_clock::now();
        // FPS 계산
        f = std::chrono::duration_cast<std::chrono::milliseconds>(Tend - Tbegin).count();
        if (f > 0.0) FPS[((Fcnt++) & 0x0F)] = 1000.0 / f;
        for(f = 0.0, i = 0; i < 16; i++) { f += FPS[i]; }
        cv::putText(frame, cv::format("FPS %0.2f", f / 16), cv::Point(10, 20), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 255));

        cv::imshow("tracking", frame);
        char esc = cv::waitKey(5);
        if (esc == 27) break;
    }

    cam.stopVideo();
    cv::destroyWindow("Video");
    return 0;
}
