cmake_minimum_required(VERSION 3.10)
project(smart_zone_camera)

# Eigen3 찾기
find_package(Eigen3 REQUIRED)

# NCNN 경로 설정
set(NCNN_INCLUDE_DIR "/home/pi/ncnn/src")
set(NCNN_LIB_DIR "/home/pi/ncnn/build/install/lib")

# lccv 경로 설정
set(LCCV_INCLUDE_DIR "/usr/local/include")  # lccv 헤더 파일 경로
set(LCCV_LIB_DIR "/usr/local/lib")  # lccv 라이브러리 경로

# OpenMP 찾기
find_package(OpenMP REQUIRED)

# pkg-config 사용하여 libcamera 경로 찾기
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

# OpenCV 경로 찾기
find_package(OpenCV REQUIRED)

# include 디렉토리 추가
include_directories(
    inc 
    ${GST_INCLUDE_DIRS} 
    ${GSTRTSP_INCLUDE_DIRS} 
    ${OpenCV_INCLUDE_DIRS}  # OpenCV 경로 추가
    ${LIBCAMERA_INCLUDE_DIRS}  # libcamera 경로 자동 설정
    ${NCNN_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIR}  # Eigen3 헤더 파일 경로 자동 설정
    /home/pi/ncnn/build/install/include/ncnn  # NCNN 소스 디렉토리 추가
)

pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)

link_directories(${NCNN_LIB_DIR})
link_directories(${LCCV_LIB_DIR})

# 소스 파일 목록 설정
set(SOURCE_FILES
    src/BYTETracker.cpp
    src/STrack.cpp
    src/kalmanFilter.cpp
    src/lapjv.cpp
    src/utils.cpp
    src/yoloX.cpp
    src/SmartZoneCamera.cpp
    src/Piframe.cpp
    src/area.cpp
    src/gstreamerServer.cpp
    src/httpClient.cpp
    src/httpServer.cpp
    main.cpp
)

# 실행 파일 생성
add_executable(smart_zone_camera ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(smart_zone_camera 
    ${GST_LIBRARIES} 
    ${GSTRTSP_LIBRARIES} 
    ${OpenCV_LIBRARIES}  # OpenCV 라이브러리 링크 추가
    ${LIBCAMERA_LIBRARIES}  # libcamera 라이브러리 추가
    ${OpenMP_CXX_LIBRARIES}  # OpenMP 라이브러리 링크
    ${LCCV_LIB_DIR}/liblccv.so  # lccv 라이브러리 명시적으로 지정
    ${NCNN_LIB_DIR}/libncnn.a  # ncnn 라이브러리 명시적으로 지정
    gstapp-1.0
)

# C++17 표준 사용
target_compile_features(smart_zone_camera PRIVATE cxx_std_17)
