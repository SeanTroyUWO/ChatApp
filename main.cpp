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

    /* Create TCP connection provider */
    const char *portNumStr = std::getenv("PORT");
    if(portNumStr == nullptr)
    {
        std::cerr << "bruh moment frfr" << std::endl;
    }
    uint16_t portNum = atoi(portNumStr);

    app.port(portNum).multithreaded().run();

    return 0;
}
