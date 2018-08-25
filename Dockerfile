FROM resin/rpi-raspbian

RUN [ "cross-build-start" ]

RUN apt-get update \
  && apt-get install -y git cmake build-essential libusb-1.0-0-dev libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
  && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/ && mkdir kinect

# Install libfreenect
RUN cd /usr/src/kinect \
  && git clone https://github.com/OpenKinect/libfreenect \
  && cd libfreenect \
  && mkdir build \
  && cd build \
  && cmake -L .. \
  && make \
  && make install

# Install pistache
RUN cd /usr/src/kinect \
  && git clone https://github.com/oktal/pistache.git \
  && cd pistache \
  && git submodule update --init \
  && mkdir build \
  && cd build \
  && cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. \
  && make \
  && make install

RUN [ "cross-build-end" ]

CMD ["tail", "-f", "/dev/null"]