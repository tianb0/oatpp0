#include <memory>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>

// begin DTO code generation
#include OATPP_CODEGEN_BEGIN(DTO)

// message data-transfer-object
class MessageDto : public oatpp::DTO {

  DTO_INIT(MessageDto, DTO /* extends */)

  DTO_FIELD(Int32, statusCode); // status code field
  DTO_FIELD(String, message);   // message field
};

// end DTO code generation
#include OATPP_CODEGEN_END(DTO)

// request handler
class HandlerHello : public oatpp::web::server::HttpRequestHandler {
public:

  // handle incoming request and return outgoing response
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
    return ResponseFactory::createResponse(Status::CODE_200, "hello world");
  }
};

// custom request handler, json dto
class Handler : public oatpp::web::server::HttpRequestHandler {
private:
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;
public:
  /**
  * Constructor with object mapper
  * @param ObjectMapper - object mapper used to serialize objects.
  */
  Handler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper)
    : m_objectMapper(objectMapper)
  {}

  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
    auto message = MessageDto::createShared();
    message->statusCode = 1024;
    message->message = "hello dto";
    return ResponseFactory::createResponse(Status::CODE_200, message, m_objectMapper);
  }
};

void run() {

  // json objcet mapper
  auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

  // create router for http requests routing
  auto router = oatpp::web::server::HttpRouter::createShared();

  // route GET /hello
  router->route("GET", "/hello", std::make_shared<HandlerHello>());

  router->route("GET", "/hi", std::make_shared<Handler>(objectMapper));

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
