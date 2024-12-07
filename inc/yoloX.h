#ifndef YOLOX_H
#define YOLOX_H

#include <opencv2/opencv.hpp>
#include <ncnn/net.h>
#include "BYTETracker.h"  // BYTETracker 관련 코드 포함

struct GridAndStride
{
    int grid0;
    int grid1;
    int stride;
};

// YOLOX 객체 탐지기 클래스
class YoloX {
public:
    YoloX();
    ~YoloX();

    // YOLOX 모델 초기화 함수
    void init(const std::string& param_file, const std::string& model_file);

    // 객체 탐지 함수
    int detect_yolox(const cv::Mat& bgr, std::vector<Object>& objects);

private:
    ncnn::Net yolox;  // YOLOX 네트워크
    int target_size = 416;  // 입력 이미지의 크기
    const float prob_threshold = 0.25f;  // 객체 탐지 확률 임계값
    const float nms_threshold = 0.45f;   // NMS 임계값
};

#endif // YOLOX_H
