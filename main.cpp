#include <lccv.hpp>
#include <opencv2/opencv.hpp>
#include "yoloV8.h"


YoloV8 yolov8;
int main()
{
    std::cout<<"Sample program for LCCV video capture"<<std::endl;
    std::cout<<"Press ESC to stop."<<std::endl;
    cv::Mat image(1280, 960, CV_8UC3, cv::Scalar(255));
    lccv::PiCamera cam;
    cam.options->video_width=1280;
    cam.options->video_height=960;
    cam.options->framerate=15;
    cam.options->verbose=true;
    cv::namedWindow("Video",cv::WINDOW_NORMAL);
    yolov8.load(640);
    std::vector<Object> object;
    cam.startVideo();
    int ch=0;
    while(ch!=27){
        if(!cam.getVideoFrame(image,1000)){
            std::cout<<"Timeout error"<<std::endl;
        }
        yolov8.detect(image, object);
        yolov8.draw(image, object);
        if(!image.empty()){
            cv::imshow("Video",image);
            ch=cv::waitKey(10);
        }
        else{
            std::cerr << "no image" << std::endl;
            return -1;
        }
    }
    cam.stopVideo();
    cv::destroyWindow("Video");
}