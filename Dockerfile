FROM resin/rpi-raspbian

RUN apt-get update \
  && apt-get install -y git cmake build-essential libusb-1.0-0-dev libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/

# Install libfreenect
RUN git clone https://github.com/OpenKinect/libfreenect \
  && cd libfreenect \
  && mkdir build \
  && cd build \
  && cmake -L .. \
  && make -j4 \
  && make install

# Install pistache
RUN git clone https://github.com/oktal/pistache.git \
  && cd pistache \
  && git submodule update --init \
  && mkdir build \
  && cd build \
  && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. \
  && make -j4 \
  && make install

# Install OpenCV
RUN git clone https://github.com/opencv/opencv.git
RUN cd opencv \
  && mkdir release \
  && cd release \
  && cmake -D CMAKE_BUILD_TYPE=RELEASE \
           -D CMAKE_INSTALL_PREFIX=/usr/local \
           -D BUILD_SHARED_LIBS=OFF \
           -D BUILD_opencv_apps=OFF \
           -D BUILD_PACKAGE=OFF \
           -D BUILD_ANDROID_SERVICE=OFF \
           -D BUILD_DOCS=OFF \
           -D INSTALL_C_EXAMPLES=OFF \
           -D INSTALL_PYTHON_EXAMPLES=OFF \
           -D BUILD_TESTS=OFF \
           -D BUILD_PERF_TESTS=OFF \
           -D BUILD_EXAMPLES=OFF \
           -D BUILD_JAVA=OFF .. \
  && make -j8 \
  && make install

RUN mkdir kinect-server
COPY api kinect-server/api
COPY model kinect-server/model
COPY lib kinect-server/lib
COPY ["main.cpp", "CMakeLists.txt", "kinect-server/"]
RUN cd kinect-server \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j4

RUN export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"
RUN ldconfig

CMD ["./kinect-server/build/kinect_server"]