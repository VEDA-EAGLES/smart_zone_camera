cmake_minimum_required(VERSION 3.10)
project(smart_zone_camera)

# include 디렉토리 추가
include_directories(include)

# 소스 파일 목록
set(SOURCE_FILES
    main.cpp
    source/gstreamerServer.cpp
    source/httpClient.cpp
    source/httpServer.cpp
)

# 실행 파일 생성
# find_package(nlohmann-json 3.2.0 REQUIRED)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(GST REQUIRED gstreamer-1.0)
# pkg_check_modules(GSTRTSP REQUIRED gstreamer-rtsp-server-1.0)

add_executable(smart_zone_camera ${SOURCE_FILES})

# target_link_libraries(smart_zone_camera
#     PkgConfig::gtk3
#     PkgConfig::gstreamer
#     PkgConfig::gstreamer-sdp
#     PkgConfig::gstreamer-app
#     PkgConfig::gstreamer-video
# )

# C++11 표준 사용
target_compile_features(smart_zone_camera PRIVATE cxx_std_11)