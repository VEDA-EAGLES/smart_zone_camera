cmake_minimum_required(VERSION 3.10)
project(smart_zone_camera)

# 라이브러리 탐색
find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)
find_package(OpenMP REQUIRED)

# 소스 파일 목록
set(SOURCE_FILES
    source/yoloV8.cpp
    source/lccv.cpp
    source/libcamera_app.cpp
    source/libcamera_app_options.cpp
    main.cpp
)

# 실행 파일 생성
add_executable(smart_zone_camera ${SOURCE_FILES})

# include 디렉토리 추가
target_include_directories(smart_zone_camera PRIVATE
    /usr/include/opencv4
    /usr/include/libcamera
    /usr/local/include/ncnn
    include
)

# 라이브러리 디렉토리 추가
target_link_directories(smart_zone_camera PRIVATE
    /usr/local/lib/ncnn
)

# 라이브러리 링크
target_link_libraries(smart_zone_camera
    ${OpenCV_LIBS}
    ${LIBCAMERA_LIBRARIES}
    ncnn
    ${OpenMP_CXX_LIBRARIES}
)

# C++17 표준 사용
target_compile_features(smart_zone_camera PRIVATE cxx_std_17)
target_compile_options(smart_zone_camera PRIVATE ${OpenMP_CXX_FLAGS})
