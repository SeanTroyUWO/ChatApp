#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <string>
#include <sstream>

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

    std::cout << "url is " << url << std::endl;

    pqxx::connection sql{url};

    CROW_ROUTE(app, "/")([&sql](){
        std::cout << "hit hello world" << std::endl;
        if (sql.is_open()) {
           std::cout << "Successfully connected to: " << sql.dbname() << std::endl;
        }
        return "Hello world";
    });

    CROW_ROUTE(app, "/users")([&sql](){
        std::cout << "hit users" << std::endl;
        pqxx::nontransaction txn(sql);

        pqxx::result result = txn.exec("SELECT id, username, email FROM users");

        std::stringstream returnVal;
        for (auto const &row : result)
        {
            returnVal << "ID: " << row["id"].as<int>() << " | "
                      // Handling potential NULL values safely
                      << "Name: " << row["username"].as<std::string>("") << " | "
                      << "Email: " << row["email"].as<std::string>("") << "\n";
        }

        return returnVal.str();
    });

    CROW_ROUTE(app, "/account")([&sql](const crow::request& req)
    {
        std::cout << "hit account" << std::endl;
        if(req.body == "")
        {
            return 0; 
        }
        std::vector params(1, req.body);

        pqxx::nontransaction txn(sql);
        pqxx::result result = txn.exec_params("SELECT COUNT(*) FROM users WHERE username = $1", req.body);

        return result.front().front().as<uint32_t>(0);
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
