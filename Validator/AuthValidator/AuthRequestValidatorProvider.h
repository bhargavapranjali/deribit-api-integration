#include "../RequestValidator.h"

class AuthValidatorFactory final: public IValidatorFactory
{
    std::unique_ptr<IRequestValidator> CreateValidator() const override 
    {
        return std::make_unique<AuthValidator>();
    }
};