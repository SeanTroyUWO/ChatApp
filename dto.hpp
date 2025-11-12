#ifndef CHATAPP_DTO_HPP
#define CHATAPP_DTO_HPP

#include "oatpp/macro/codegen.hpp"
#include "oatpp/Types.hpp"


#include OATPP_CODEGEN_BEGIN(DTO)
// class UserDto : public oatpp::DTO {
//
//     DTO_INIT(UserDto, DTO)
//
//     DTO_FIELD(String, id);
//     DTO_FIELD(String, username, "username");
//     DTO_FIELD(String, email, "email");
//     DTO_FIELD(String, password, "password");
//     DTO_FIELD(Enum<Role>::AsString, role, "role");
// };

class UserPublicFields : public oatpp::DTO
{
    DTO_INIT(UserPublicFields, DTO)
    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
};

class UserList : public oatpp::DTO
{
    DTO_INIT(UserList, DTO)
    DTO_FIELD(Vector<Int64>, ids);
    DTO_FIELD(Vector<String>, names);
};
#include OATPP_CODEGEN_END(DTO)

#endif //CHATAPP_DTO_HPP