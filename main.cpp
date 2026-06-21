#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <random>
#include <ranges>
#include <string>
#include <sstream>
#include <unordered_set>

#include "crow.h"
#include <pqxx/pqxx>

uint64_t encrypt(std::string input)
{
    uint64_t result = 0x4624748522D64145;
    for(char &c : input)
    {
        result = (uint64_t)powl(result, c) ^ (result << 1);
    }
    return result;
}

uint64_t getNewToken()
{
    // 1. Seed with a non-deterministic random device source
    static std::random_device rd;

    // 2. Initialize the 64-bit Mersenne Twister engine with the seed
    static std::mt19937_64 engine(rd());

    // 3. Define a uniform distribution over the full uint64_t range
    static std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

    // 4. Generate the random number
    uint64_t random_num = dist(engine);

    std::cout << "Random 64-bit Number: " << random_num << "\n";
    return random_num;
}

std::optional<std::vector<std::string>> getLoginParams(const std::string& input)
{
    std::cout << "hit login PATCH" << std::endl;
    if(input == "")
    {
        std::cout << "blank login" << std::endl;
        return std::nullopt;
    }
    std::cout << "with body " << input << std::endl;

    std::vector<std::string> params{};
    std::string segment;
    std::istringstream iss(input);
    int i = 0;
    while(std::getline(iss, segment, ' '))
    {
        std::cout << "param: " << segment << std::endl;
        params.push_back(segment);
        i += 1;
        if(i == 4)
        {
            return std::nullopt;
        }
    }

    if(params.size() != 2)
    {
        std::cerr << "wrong number of parameters" << std::endl;
        return std::nullopt;
    }

    return params;
}

int main()
{
    std::cout << "Starting Server" << std::endl;
    crow::SimpleApp app;

    std::unordered_set<uint64_t> goodTokens{};

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

    CROW_ROUTE(app, "/account")
    .methods(crow::HTTPMethod::GET)
    ([&sql](const crow::request& req)->std::string
    {
        std::cout << "hit account" << std::endl;
        if(req.body == "")
        {
            std::cout << "no body" << std::endl;
            std::cout << req.body << std::endl;
            return "";
        }
        std::cout << "with body " << req.body << std::endl;

        pqxx::nontransaction txn(sql);
        pqxx::result result = txn.exec_params("SELECT id FROM users WHERE username = $1 LIMIT 1", req.body);

        return result.front().front().as<std::string>("0");
    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::PATCH)
    ([&sql, &goodTokens](const crow::request& req)-> std::string
    {
        std::cout << "hit login PATCH" << std::endl;
        std::optional<std::vector<std::string>> params = getLoginParams(req.body);
        if(!params)
        {
            return std::to_string(0);
        }

        const std::string& username = (*params)[0];
        const std::string& password = (*params)[1];
        uint64_t encryptedPassword = encrypt(password);

        (*params)[1] = std::to_string(encryptedPassword);

        pqxx::nontransaction txn(sql);
        pqxx::result result = txn.exec_params("INSERT INTO users (username, password) VALUES ($1, $2)", (*params)[0], (*params)[1]);

        uint64_t newToken = getNewToken();
        std::cout << "newToken " << newToken <<  std::endl;
        goodTokens.insert(newToken);
        return std::to_string(newToken);
    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::POST)
    ([&sql, &goodTokens](const crow::request& req)-> std::string
    {
        std::cout << "hit login POST" << std::endl;

        std::optional<std::vector<std::string>> params = getLoginParams(req.body);
        if(!params)
        {
            return std::to_string(0);
        }

        const std::string& username = (*params)[0];
        const std::string& password = (*params)[1];
        uint64_t encryptedPassword = encrypt(password);

        pqxx::nontransaction txn(sql);
        pqxx::result result = txn.exec_params("SELECT password FROM users WHERE username = $1 LIMIT 1", username);
        uint64_t actualEncryptedPassword = result.front().front().as<uint64_t>();

        std::cout <<"actualEncryptedPassword: " << actualEncryptedPassword << std::endl;
        std::cout <<"encryptedPassword: " << encryptedPassword << std::endl;

        if(actualEncryptedPassword == encryptedPassword)
        {
            uint64_t newToken = getNewToken();
            std::cout << "newToken " << newToken <<  std::endl;
            goodTokens.insert(newToken);
            return std::to_string(newToken);
        }
        std::cout <<"missed " << std::endl;

        return std::to_string(0);
    });

    // CROW_WEBSOCKET_ROUTE(app, "/ws")
    // .onopen([&](crow::websocket::connection& conn){
    //     conn.
    //     do_something();
    // })
    // .onclose([&](crow::websocket::connection& conn, const std::string& reason, uint16_t){
    //         do_something();
    // })
    // .onmessage([&](crow::websocket::connection& /*conn*/, const std::string& data, bool is_binary){
    //             if (is_binary)
    //                 do_something(data);
    //             else
    //                 do_something_else(data);
    // });

    uint16_t portNum;
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
