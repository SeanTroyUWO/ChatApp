#ifndef CHATAPP_USERSERVICE_HPP
#define CHATAPP_USERSERVICE_HPP

#include "DbDelegate.hpp"
// #include "dto/PageDto.hpp"
// #include "dto/StatusDto.hpp"

#include "dto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/component.hpp"

class UserService
{
public:
    OATPP_COMPONENT(std::shared_ptr<DbDelegate>, database_); // Inject database component

    oatpp::Object<UserList> getAllUsers()
    {
        auto dbResult = database_->getAllUsers();
        // OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

        // auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<UserPublicFields>>>();
        return dbResult->fetch<oatpp::Object<UserList>>();
    }
};

#endif //CHATAPP_USERSERVICE_HPP