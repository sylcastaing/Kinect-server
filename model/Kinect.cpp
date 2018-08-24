#include "Kinect.h"

using namespace cv;
using namespace std;

namespace kinectserver {
  Kinect::Kinect() {
    freenect_sync_set_led(LED_OFF, 0);
  };

  freenect_raw_tilt_state* Kinect::getTilt() {
    freenect_sync_get_tilt_state(&state, 0);
    return state;
  }

  void Kinect::setTilt(int angle) {
    freenect_sync_set_tilt_degs(angle, 0);
  }

  void Kinect::startVideo() {
    cout << "Start video streaming" << endl;

    Mat rgbimg(FREENECTOPENCV_RGB_HEIGHT, FREENECTOPENCV_RGB_WIDTH, CV_8UC3);
    Mat tempimg(FREENECTOPENCV_RGB_HEIGHT, FREENECTOPENCV_RGB_WIDTH, CV_8UC3);

    char *data;
    unsigned int timestamp;

    while (1) {
      freenect_sync_get_video((void**)(&data), &timestamp, 0, FREENECT_VIDEO_RGB);
      memcpy(rgbimg.data, data, (FREENECTOPENCV_RGB_WIDTH+0)*(FREENECTOPENCV_RGB_HEIGHT+950));
      cvtColor(tempimg,tempimg,COLOR_HSV2BGR);
      cvtColor(rgbimg,tempimg,COLOR_RGB2BGR);
      imwrite(FILE_NAME, tempimg);
      std::this_thread::sleep_for(std::chrono::milliseconds(FREQUENCY));
    }
  }
}