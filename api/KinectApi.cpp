#include "KinectApi.h"

using namespace rapidjson;

namespace kinectserver {
  KinectApi::KinectApi(Pistache::Address addr)
    : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr))
  { };

  void KinectApi::init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options()
        .threads(thr)
        .flags(Pistache::Tcp::Options::InstallSignalHandler | Pistache::Tcp::Options::ReuseAddr);
    httpEndpoint->init(opts);
    setupRoutes();
  }

  void KinectApi::start() {
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

  void KinectApi::shutdown() {
    httpEndpoint->shutdown();
  }

  void KinectApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Get(router, "/camera", Routes::bind(&KinectApi::getImage, this));
    Routes::Get(router, "/tilt", Routes::bind(&KinectApi::getTilt, this));
    Routes::Post(router, "/tilt", Routes::bind(&KinectApi::setTilt, this));

    // Default handler, called when a route is not found
    router.addCustomHandler(Routes::bind(&KinectApi::defaultReponse, this));
  }

  void KinectApi::getImage(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    Pistache::Http::serveFile(response, kinect.getImage().c_str());
  }

  void KinectApi::getTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    freenect_raw_tilt_state* state = kinect.getTilt();

    Document document;
    document.SetObject();

    Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("accelerometer_x", state->accelerometer_x, allocator);
    document.AddMember("accelerometer_y", state->accelerometer_y, allocator);
    document.AddMember("accelerometer_z", state->accelerometer_z, allocator);
    document.AddMember("tilt_angle", state->tilt_angle, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    document.Accept(writer);

    response.send(Pistache::Http::Code::Ok, buffer.GetString());
  }

  void KinectApi::setTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    Document document;
    if (document.Parse(request.body().c_str()).HasParseError()) {
      response.send(Pistache::Http::Code::Bad_Request, "Parse error");
    }
    else {
      if (document.HasMember("tilt_angle") && document["tilt_angle"].IsNumber()) {
        int angle = document["tilt_angle"].GetInt();
        
        angle = (angle > 20) ? 20 : angle;
        angle = (angle < -20) ? -20 : angle;
  
        kinect.setTilt(angle);
        response.send(Pistache::Http::Code::Accepted, request.body());
      }
      else {
        response.send(Pistache::Http::Code::Bad_Request, "No tilt_angle in body");
      }
    }
  }

  void KinectApi::defaultReponse(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "Route not found");
  }
}
