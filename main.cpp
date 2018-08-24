#include <iostream>
#include <signal.h>
#include <thread>

#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

#include "api/KinectApi.h"
#include "model/Kinect.h"

using namespace std;
using namespace kinectserver;

bool stop(false);

void my_handler(int signal)
{
  cout << "Signal: " << signal << endl;
  stop = true;
}

int main() {
  Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8080));
  KinectApi server(addr);
  server.init(2);
  server.start();
  server.shutdown();

  return 0;
}