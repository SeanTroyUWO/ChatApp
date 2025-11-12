#ifndef CHATAPP_OBJECT_MAPPER_HPP
#define CHATAPP_OBJECT_MAPPER_HPP


#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"

class ServiceComponent
{
public:
    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([]
    {
        std::shared_ptr<oatpp::json::ObjectMapper> mapper = std::make_shared<oatpp::json::ObjectMapper>();
        // mapper->serializerConfig().useBeautifier = true;
        // mapper->serializerConfig()->escapeFlags = 0;
        return mapper;
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<DbDelegate>, dbDelegate)([]
    {
        const char *dbUrl = std::getenv("DATABASE_URL");
        if(dbUrl == nullptr)
        {
            std::cerr << "bruh moment frfr" << std::endl;
        }
        auto connectionProvider = std::make_shared<oatpp::postgresql::ConnectionProvider>(dbUrl);

        /* Create database-specific ConnectionPool */
        auto connectionPool = oatpp::postgresql::ConnectionPool::createShared(connectionProvider,
                                                                              10 /* max-connections */,
                                                                              std::chrono::seconds(5)
                                                                              /* connection TTL */);

        /* Create database-specific Executor */
        auto executor = std::make_shared<oatpp::postgresql::Executor>(connectionPool);

        /* Create MyClient database client */
        return std::make_shared<DbDelegate>(executor);
    }());

    // /**
    //  *  Create ConnectionProvider component which listens on the port
    //  */
    // OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    //   OATPP_COMPONENT(oatpp::Object<ConfigDto>, config); // Get config component
    //   return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", config->port, oatpp::network::Address::IP_4});
    // }());
    //
    // /**
    //  *  Create Router component
    //  */
    // OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    //   return oatpp::web::server::HttpRouter::createShared();
    // }());
    //
    // /**
    //  *  Create ConnectionHandler component which uses Router component to route requests
    //  */
    // OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    //   OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    //   OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component
    //
    //   auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
    //   connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));
    //   return connectionHandler;
    // }());
};

#endif //CHATAPP_OBJECT_MAPPER_HPP
