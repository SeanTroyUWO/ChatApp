#include <cstdlib>

#include <iostream>

#include "oatpp/Environment.hpp" // Or oatpp/Environment.hpp for older versions

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "controller.hpp"

#include "ObjectMapper.hpp"

#include "oatpp-1.4.0/oatpp-postgresql/oatpp-postgresql/orm.hpp" //< SQLite adapter for oatpp ORM

#include OATPP_CODEGEN_BEGIN(DbClient) ///< Begin code-gen section

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section

// class AppComponent {
// public:
//
//     /**
//      * Create DbClient component.
//      * SQLite is used as an example here. For other databases declaration is similar.
//      */
//     OATPP_CREATE_COMPONENT(std::shared_ptr<MyClient>, MyDbClient)([] {
//
//       /* Create database-specific ConnectionProvider */
//       auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>("/path/to/database.sqlite");
//
//       /* Create database-specific Executor */
//       auto executor = std::make_shared<oatpp::postgresql::Executor>(connectionProvider);
//
//       /* Create MyClient database client */
//       return std::make_shared<MyClient>(executor);
//
//     }());
//
// };

// class LandingHandler : public oatpp::web::server::HttpRequestHandler
// {
// public:
//
//     std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
//     {
//         return ResponseFactory::createResponse(Status::CODE_200, "wat up");
//     }
// };
//
// class LandingHandler : public oatpp::web::server::HttpRequestHandler
// {
// public:
//
//     std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
//     {
//         return ResponseFactory::createResponse(Status::CODE_200, "wat up");
//     }
// };

void run()
{
    ServiceComponent serviceComponent;
    UserService userService;

    /* Create Router for HTTP requests routing */
    auto router = oatpp::web::server::HttpRouter::createShared();

    router->addController(Controller::createShared());

    /* Create HTTP connection handler with router */
    auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);

    /* Create TCP connection provider */
    const char *portNumStr = std::getenv("PORT");
    if(portNumStr == nullptr)
    {
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

int main()
{
    std::cout << "Starting Server" << std::endl;

    oatpp::Environment::init();
    run();
    oatpp::Environment::destroy();

    return 0;
}
