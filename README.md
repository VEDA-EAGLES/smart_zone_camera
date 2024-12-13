# Smart Zone Camera Project

Smart Zone 프로젝트는 라즈베리 파이 5와 카메라 모듈을 이용하여 실시간 모니터링 및 의미 있는 데이터를 모아 그래프로 보여 주는 프로젝트 입니다. 

Smart Zone Camera는 라즈베리 파이 5와 카메라 모듈을 이용하여 영상정보를 받아와 가공 후, 서버와 클라이언트로 데이터를 전송합니다.

### 작동 방식
Gstreamer의 파이프라인을 사용하여 라즈베리 파이의 카메라로 촬영한 영상정보를 로컬호스트(본인)로 udp 전송, 받은 영상정보를 opencv로 가공하여 rtsp 스트리밍하는 프로그램입니다. 미리 설정된 서버와 HTTP 통신으로 데이터를 Send/Receive하며 제어됩니다.  

## GitHub Repository

- [Smart Zone Camera](https://github.com/VEDA-EAGLES/smart_zone_camera)

## 의존성

### YOLOvX
[YOLOvX-Tracking-ncnn-RPi_64-bit](https://github.com/Qengineering/YoloX-Tracking-ncnn-RPi_64-bit)

YOLOvX를 사용하여 객체 감지 및 추적을 구현하려면 아래와 같은 의존성들이 필요합니다:

- **OpenCV 4.6.0**
  - 이미지 처리 작업을 위해 OpenCV가 필요합니다.
  - OpenCV 4.6.0을 설치하려면 다음 명령어를 사용합니다:
    ```bash
    sudo apt install libopencv-dev
    ```

- **ncnn**
  - ncnn은 엣지 디바이스에서 신경망 추론을 효율적으로 처리하는 고성능 프레임워크입니다.
  - Raspberry Pi에 ncnn을 설치하려면 아래의 지침을 따르세요:  
    [Install ncnn on Raspberry Pi](https://qengineering.eu/install-ncnn-on-raspberry-pi-4.html)

    ### ncnn 설치 방법 (Raspberry Pi 64-bit)

    이 프로젝트는 `ncnn`을 사용하여 효율적인 신경망 추론을 처리합니다. Raspberry Pi 64-bit (aarch64) 환경에서 `ncnn`을 설치하는 방법은 아래와 같습니다:

    #### 1. 시스템 업데이트
    시스템을 업데이트하여 최신 상태로 유지합니다.
    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    ```

    #### 2. 필요한 의존성 설치
    `ncnn` 빌드를 위한 필수 패키지들을 설치합니다.
    ```bash
    sudo apt-get install cmake wget
    sudo apt-get install build-essential gcc g++
    sudo apt-get install libprotobuf-dev protobuf-compiler
    sudo apt-get install libeigen3-dev
    ```

    #### 3. `ncnn` 다운로드
    `ncnn` 소스 코드를 GitHub에서 클론하여 다운로드합니다.
    ```bash
    git clone --depth=1 https://github.com/Tencent/ncnn.git
    ```

    #### 4. `ncnn` 빌드 및 설치
    `ncnn` 디렉토리로 이동 후 빌드를 시작합니다.

    ```bash
    cd ncnn
    mkdir build
    cd build
    ```

    64-bit Raspberry Pi (aarch64) 환경에서 `ncnn`을 빌드하기 위해 아래 명령어를 실행합니다:
    ```bash
    cmake -D NCNN_DISABLE_RTTI=OFF -D NCNN_BUILD_TOOLS=ON \
    -D CMAKE_TOOLCHAIN_FILE=../toolchains/aarch64-linux-gnu.toolchain.cmake ..
    ```

    빌드 과정에서 병렬 처리를 위해 `-j4` 옵션을 사용합니다:
    ```bash
    make -j4
    ```

    빌드가 완료되면 설치를 진행합니다:
    ```bash
    make install
    ```

    #### 5. 라이브러리 및 헤더 파일 복사
    빌드된 `ncnn` 라이브러리와 헤더 파일을 시스템 디렉토리로 복사합니다:
    ```bash
    sudo mkdir /usr/local/lib/ncnn
    sudo cp -r install/include/ncnn /usr/local/include/ncnn
    sudo cp -r install/lib/libncnn.a /usr/local/lib/ncnn/libncnn.a
    ```

    이제 `ncnn` 설치가 완료되었습니다. 설치가 끝난 후 YOLOvX 추적 모델을 사용할 수 있습니다.

### LCCV
LCCV는 실시간으로 카메라 데이터를 캡처하고 처리하는 데 사용됩니다.

- **libCamera**
  - libCamera는 카메라 장치를 제어하는 풍부한 API를 제공합니다.
  - libCamera를 설치하려면 아래 명령어를 사용합니다:
    ```bash
    sudo apt install libcamera-apps
    sudo apt install libcamera-dev
    ```

- **LCCV**
  #### 1. `LCCV` 다운로드
    `LCCV` 소스 코드를 GitHub에서 클론하여 다운로드합니다.
    ```bash
    git clone https://github.com/kbarni/LCCV.git
    ```

  #### 2. `LCCV` 빌드 및 설치
    `LCCV` 디렉토리로 이동 후 빌드를 시작합니다.

    ```bash
    cd LCCV
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
    ```

### 카메라 / 서버 의존성

- **httplib (헤더 온리 라이브러리)**
  - [httplib GitHub 리포지토리](https://github.com/yhirose/cpp-httplib)에서 `httplib.h` 파일을 다운로드하여 프로젝트에 추가하고 링크하여 사용합니다.

- **nlohmann json (헤더 온리 라이브러리)**
  - [nlohmann/json GitHub 리포지토리](https://github.com/nlohmann/json/tree/develop/single_include/nlohmann)에서 `single_include/nlohmann/json.hpp` 파일을 다운로드하여 프로젝트에 추가하고 링크하여 사용합니다.

- **gstreamer**
  - GStreamer는 멀티미디어 프레임워크로, 실시간 비디오 스트리밍과 처리를 지원합니다.
  - GStreamer를 설치하려면 아래 명령어를 사용합니다:
    ```bash
    sudo apt-get install --no-install-recommends -y \
    libgstrtspserver-1.0-dev \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-good \
    gstreamer1.0-x \
    gstreamer1.0-libav \
    gstreamer1.0-libcamera \
    gstreamer1.0-tools
    
    sudo apt-get install libglib2.0-dev libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa libgstrtspserver-1.0-0
    ```

## 설치

1. 리포지토리를 클론합니다:
   ```bash
   git clone https://github.com/VEDA-EAGLES/smart_zone_camera.git

2. 카메라와 연결될 서버를 설정합니다.
   ```bash 
   cd smart_zone_camera/inc/define.h => define.h의 SERVER_IP를 "<서버 IP>"로 변경
   
4. smart_zone_camera 경로의 셀스크립트에 카메라의 IP를 같이 입력하여 프로그램을 실행합니다.
   ```bash
   ./smart_zone_camera <IP ADDRESS>
