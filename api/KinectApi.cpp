#include "KinectApi.h"

namespace kinectserver {
  KinectApi::KinectApi(Pistache::Address addr)
    : httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr))
  { };

  void KinectApi::init(size_t thr = 2) {
    auto opts = Pistache::Http::Endpoint::options()
        .threads(thr)
        .flags(Pistache::Tcp::Options::InstallSignalHandler);
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

    Routes::Get(router, "/tilt", Routes::bind(&KinectApi::getTilt, this));
    Routes::Get(router, "/tilt2", Routes::bind(&KinectApi::changeTilt, this));

    // Default handler, called when a route is not found
    router.addCustomHandler(Routes::bind(&KinectApi::defaultReponse, this));
  }

  void KinectApi::getTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Ok, "Do some magic\n");
  }

  void KinectApi::changeTilt(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {

  }

  void KinectApi::defaultReponse(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) {
    response.send(Pistache::Http::Code::Not_Found, "The requested method does not exist");
  }
}
