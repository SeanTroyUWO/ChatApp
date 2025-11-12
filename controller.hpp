#ifndef CHATAPP_CONTROLLER_HPP
#define CHATAPP_CONTROLLER_HPP

#include <oatpp/macro/component.hpp>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/json//ObjectMapper.hpp"
#include "oatpp/macro/codegen.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/codegen/api_controller/base_define.hpp"
#include "userService.hpp"

#include "DbDelegate.hpp"
#include "dto.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(Role, v_int32,
     VALUE(GUEST, 0, "ROLE_GUEST"),
     VALUE(ADMIN, 1, "ROLE_ADMIN")
)

#include OATPP_CODEGEN_END(DTO)


#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen
class Controller : public oatpp::web::server::api::ApiController
{
public:
    Controller(const std::shared_ptr<ObjectMapper>& apiObjectMapper)
      : oatpp::web::server::api::ApiController(apiObjectMapper)
    {
        //
    }

    UserService userService_; // Create user service.

    static std::shared_ptr<Controller> createShared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, apiObjectMapper) // Inject objectMapper component here as default parameter
    )
    {
        return std::make_shared<Controller>(apiObjectMapper);
    }

    ENDPOINT_INFO(getAllUsers)
    {
        // info->summary = "Get";
        info->addResponse<oatpp::Object<UserList>>(Status::CODE_200, "application/json");
        // info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users", getAllUsers)
    {
        return createDtoResponse(Status::CODE_200, userService_.getAllUsers());
    }

    ENDPOINT_INFO(landing)
    {
        // info->summary = "Get";
        info->addResponse<oatpp::Object<UserList>>(Status::CODE_200, "application/json");
        // info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "/", landing)
    {
        return ResponseFactory::createResponse(Status::CODE_200, "does this work?");
    }
};

#include OATPP_CODEGEN_BEGIN(ApiController) //<- End Codegen

#endif //CHATAPP_CONTROLLER_HPP