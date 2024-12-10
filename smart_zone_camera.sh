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
if [ $# -lt 1 ]; then  # 인자 개수가 1보다 작으면 (인자가 없으면)
  echo "Usage: $0 <IP address>"  # 사용법 출력
  exit 1
else
  ./smart_zone_camera "$@" || {  # 인자가 있으면 실행
    echo "smart_zone_camera fail"
    exit 1
  }
fi

# 원래 디렉토리로 이동
cd "$CURRENT_DIR"