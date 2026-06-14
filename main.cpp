#include <cstdlib>
#include <cstdint>

#include <iostream>

#include "crow.h"

int main()
{
    std::cout << "Starting Server" << std::endl;
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
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
