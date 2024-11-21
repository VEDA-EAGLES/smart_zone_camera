#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include "yoloV8.h"

YoloV8 yolov8;

int main()
{
    std::cout << "Sample program for LCCV video capture" << std::endl;
    std::cout << "Press ESC to stop." << std::endl;
    cv::Mat image(1280, 960, CV_8UC3, cv::Scalar(255));
    lccv::PiCamera cam;
    cam.options->video_width = 1280;
    cam.options->video_height = 960;
    cam.options->framerate = 15;
    cam.options->verbose = true;
    cv::namedWindow("Video", cv::WINDOW_NORMAL);
    yolov8.load(640);
    std::vector<Object> object;
    cam.startVideo();
    int ch = 0;
    cv::Ptr<cv::Tracker> tracker;
    cv::Rect trackingBox;
    bool tracker_init=false;

    while (ch != 27) {
        if (!cam.getVideoFrame(image, 1000)) {
            std::cout << "Timeout error" << std::endl;
        }
        if (!image.empty()) {
            yolov8.detect(image, object);
            yolov8.draw(image, object);
            if (tracker_init) {
                bool ok = tracker->update(image, trackingBox);
                if (ok) {
                    cv::rectangle(image, trackingBox, cv::Scalar(0, 255, 0), 2);
                } else {
                    cv::putText(image, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);
                    tracker_init=false;
                }
            } else {
                if(!object.empty() && object[0].label==0) {
                    std::cout << "tracking start" << std::endl;
                    tracker = cv::TrackerKCF::create();
                    trackingBox = cv::Rect(static_cast<int>(object[0].rect.x), static_cast<int>(object[0].rect.y), static_cast<int>(object[0].rect.width), static_cast<int>(object[0].rect.height));

                    tracker->init(image, trackingBox);
                    cv::rectangle(image, trackingBox, cv::Scalar( 255, 0, 0 ), 2);
                    tracker_init=true;
                }
            }

            cv::imshow("Video", image);
            ch = cv::waitKey(10);
        }
        else {
            std::cerr << "no image" << std::endl;
            return -1;
        }
    }

    cam.stopVideo();
    cv::destroyWindow("Video");
}
