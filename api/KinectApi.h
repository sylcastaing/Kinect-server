#ifndef KinectApi_H_
#define KinectApi_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>

#include "../lib/rapidjson/document.h"
#include "../lib/rapidjson/writer.h"
#include "../lib/rapidjson/stringbuffer.h"

#include "../model/Kinect.h"

namespace kinectserver {
  class KinectApi {
    public:
      KinectApi(Pistache::Address adr);
      virtual ~KinectApi() {};
      void init(size_t thr);
      void start();
      void shutdown();

    private:
      void setupRoutes();

      void getImage(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
      void getTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
      void setTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);
      void defaultReponse(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response);

      std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;
      Pistache::Rest::Router router;

      Kinect kinect;
  };
}

#endif