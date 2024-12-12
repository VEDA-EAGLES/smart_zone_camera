cmake_minimum_required(VERSION 3.10)
project(smart_zone_camera)

# 필요한 패키지 찾기
find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)

# pkg-config를 사용하여 필요한 모듈 찾기
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)

# 포함 디렉토리 설정
include_directories(
        inc
        ${GST_INCLUDE_DIRS}
        ${GSTRTSP_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${LIBCAMERA_INCLUDE_DIRS}
        /usr/local/include/ncnn/
        ${EIGEN3_INCLUDE_DIR}
)

# 라이브러리 디렉토리 설정
link_directories(
        /usr/local/lib/ncnn/libncnn.a
        /usr/local/lib
)

# 소스 파일 목록
set(SOURCE_FILES
        src/BYTETracker.cpp
        src/STrack.cpp
        src/kalmanFilter.cpp
        src/lapjv.cpp
        src/utils.cpp
        src/yoloX.cpp
        src/SmartZoneCamera.cpp
        src/area.cpp
        src/gstreamerServer.cpp
        src/httpClient.cpp
        src/httpServer.cpp
        main.cpp
)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(${PROJECT_NAME}
        ${GST_LIBRARIES}
        ${GSTRTSP_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${LIBCAMERA_LIBRARIES}
        ${OpenMP_CXX_LIBRARIES}
        /usr/local/lib/liblccv.so
        /usr/local/lib/ncnn/libncnn.a
        gstapp-1.0
)

# 컴파일 옵션 설정
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
