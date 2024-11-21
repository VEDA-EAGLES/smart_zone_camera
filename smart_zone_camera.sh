#!/bin/bash

# 현재 디렉토리 확인
CURRENT_DIR=$(pwd)

# build 디렉토리로 이동
cd build || {
    mkdir build
    cd build
}

# CMake 실행
cmake .. || {
  echo "cmake fail"
  exit 1
}

# Make 실행
make || {
  echo "make fail"
  exit 1
}

# smart_zone_camera 실행
./smart_zone_camera || {
  echo "smart_zone_camera fail"
  exit 1
}

# 원래 디렉토리로 이동
cd "$CURRENT_DIR"