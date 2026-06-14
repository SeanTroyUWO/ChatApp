#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <string>

#include "crow.h"
#include <pqxx/pqxx>

int main()
{
    std::cout << "Starting Server" << std::endl;
    crow::SimpleApp app;

    const char* url = std::getenv("DATABASE_URL");
    if(url == nullptr)
    {
        std::cerr << "url missing" << std::endl;
    }

    pqxx::connection sql{url};

    CROW_ROUTE(app, "/")([&sql](){
        std::cout << "hit hello world" << std::endl;
        if (sql.is_open()) {
           std::cout << "Successfully connected to: " << sql.dbname() << std::endl;
        }
        return "Hello world";
    });

    uint16_t portNum;
    /* Create TCP connection provider */
    const char *portNumStr = std::getenv("PORT");
    if(portNumStr == nullptr)
    {
        std::cerr << "bruh moment frfr" << std::endl;
        portNum = 8080;
    }
    else
    {
        portNum = atoi(portNumStr);
    }

    app.port(portNum).multithreaded().run();

    return 0;
}
