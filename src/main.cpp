#include <oatpp/core/base/Environment.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

void run() {

  // create router for http requests routing
  auto router = oatpp::web::server::HttpRouter::createShared();

  // create http  connection handler with router
  auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

  // create tcp connection provider
  auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000, oatpp::network::Address::IP_4});

  // create server which takes provided tcp connections and passes them to http connection handler
  oatpp::network::Server server(connectionProvider, connectionHandler);

  // print info about server port
  OATPP_LOGI("Oat0", "Server running on port %s", connectionProvider->getProperty("port").getData());

  // run server
  server.run();
}

int main() {
  // init oatpp env
  oatpp::base::Environment::init();

  // run app
  run();

  // destroy oatpp env
  oatpp::base::Environment::destroy();

  return 0;
}