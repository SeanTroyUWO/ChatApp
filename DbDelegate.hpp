#ifndef CHATAPP_DBDELEGATE_HPP
#define CHATAPP_DBDELEGATE_HPP

#include <oatpp/codegen/DbClient_define.hpp>

#include "oatpp/Environment.hpp" // Or oatpp/Environment.hpp for older versions

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"

#include "oatpp-1.4.0/oatpp-postgresql/oatpp-postgresql/orm.hpp" //< SQLite adapter for oatpp ORM

#include OATPP_CODEGEN_BEGIN(DbClient) //<- Begin Codegen
class DbDelegate : public oatpp::orm::DbClient {
public:


    DbDelegate(const std::shared_ptr<oatpp::orm::Executor>& executor)
      : oatpp::orm::DbClient(executor)
    {}

    QUERY(getAllUsers,
          "SELECT (id, username) FROM users;");

    // QUERY(getUserByName,
    //       "SELECT * FROM users WHERE username=:username;",
    //       PARAM(oatpp::String, username))

  };
#include OATPP_CODEGEN_END(DbClient) //<- End Codegen


#endif //CHATAPP_DBDELEGATE_HPP