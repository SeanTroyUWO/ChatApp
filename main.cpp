#include <cstdlib>

#include <iostream>

#include "oatpp/Environment.hpp" // Or oatpp/Environment.hpp for older versions

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

/**
 * Custom Request Handler
 */
class Handler : public oatpp::web::server::HttpRequestHandler {
public:

    /**
     * Handle incoming request and return outgoing response.
     */
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {
        return ResponseFactory::createResponse(Status::CODE_200, "wat up");
    }

};

void run() {

    /* Create Router for HTTP requests routing */
    auto router = oatpp::web::server::HttpRouter::createShared();

    /* Route GET - "/hello" requests to Handler */
    router->route("GET", "/", std::make_shared<Handler>());

    /* Create HTTP connection handler with router */
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    /* Create TCP connection provider */
    const char* portNumStr = std::getenv("PORT");
    if(portNumStr == nullptr) {
        std::cerr << "bruh moment frfr" << std::endl;
    }
    v_uint16 portNum = atoi(portNumStr);
    auto connectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
        {"0.0.0.0", portNum, oatpp::network::Address::IP_4}
        );

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Print info about server port */
    // OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

    /* Run server */
    server.run();
}

int main() {


    std::cout << "Starting Server" << std::endl;
    /* Init oatpp Environment */
    oatpp::Environment::init();

    /* Run App */
    run();

    /* Destroy oatpp Environment */
    oatpp::Environment::destroy();

    return 0;

}