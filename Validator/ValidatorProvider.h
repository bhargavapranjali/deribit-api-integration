#pragma once
#include "RequestValidator.h"

class IValidatorFactory
{
    public:
        virtual std::unique_ptr<IRequestValidator> CreateValidator() const = 0;
        virtual ~IValidatorFactory() = default;
};