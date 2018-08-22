#include <iostream>
#include <signal.h>

#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

#include "api/KinectApi.h"

using namespace std;
using namespace kinectserver;

bool stop(false);

void signalHandler(int signal)
{
  cout << "Signal: " << signal << endl;
  stop = true;
}

int main() {
  signal(SIGINT, signalHandler);
  signal(SIGILL, signalHandler);
  signal(SIGKILL, signalHandler);

  Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8080));
  
  KinectApi server(addr);
  server.init(2);
  server.start();
  
  cout << "Server started" << endl;

  server.shutdown();

  cout << "Exiting" << endl;

  return 0;
}