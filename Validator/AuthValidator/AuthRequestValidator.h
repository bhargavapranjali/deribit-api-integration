#include "../RequestValidator.h"

class AuthValidator final: public IRequestValidator
{
    RequestValidationResult validateRequest(json::value& requestBodyJson) const override
    {
        string errorMessage = "";
        this->addErrorIfFieldInvalid(requestBodyJson,"clientId",errorMessage);
        this->addErrorIfFieldInvalid(requestBodyJson,"clientSecret",errorMessage);

        json::value errorResponse;
        if(!errorMessage.empty()) {
            errorResponse[U("error_message")] = web::json::value::string(errorMessage);
        }
        return RequestValidationResult(errorMessage.empty(), errorResponse);
    }
};