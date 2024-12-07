cmake_minimum_required(VERSION 3.10)
project(smart_zone_camera)

# include 디렉토리 추가
include_directories(inc 
                    ${GST_INCLUDE_DIRS} 
                    ${GSTRTSP_INCLUDE_DIRS} 
                    ${OpenCV_INCLUDE_DIRS} 
                    /usr/local/include/ncnn
                    /usr/local/include/eigen3
                    /usr/include/libcamera
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
    src/Piframe.cpp
    src/area.cpp
    main.cpp
    src/gstreamerServer.cpp
    src/httpClient.cpp
    src/httpServer.cpp
    src/area.cpp
)

# 실행 파일 생성
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0)
pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)
find_package(OpenMP REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
find_library(LCCV_LIBRARY NAMES lccv PATHS /usr/local/lib)

add_executable(smart_zone_camera ${SOURCE_FILES})

# 라이브러리 링크
target_link_libraries(smart_zone_camera 
    ${GST_LIBRARIES} 
    ${GSTRTSP_LIBRARIES} 
    ${OpenCV_LIBRARIES}
    ${LIBCAMERA_LIBRARIES}
    ncnn
    ${OpenMP_CXX_LIBRARIES}
    ${LCCV_LIBRARY}
)

# 라이브러리 디렉토리 추가
target_link_directories(smart_zone_camera PRIVATE
    /usr/local/lib/ncnn
)

# C++17 표준 사용
target_compile_features(smart_zone_camera PRIVATE cxx_std_17)