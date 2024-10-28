#pragma once
#include "../utils.h"

struct RequestValidationResult
{
    bool isRequestValid;
    web::json::value errorMessage;

    RequestValidationResult(bool isRequestValid, json::value errorMessage): 
        isRequestValid(isRequestValid), errorMessage(errorMessage){};
};

class IRequestValidator 
{
    public:
        virtual RequestValidationResult validateRequest(json::value& requestBodyJson) const = 0;
        virtual ~IRequestValidator() = default;
    protected:
        void addErrorIfFieldInvalid(json::value& requestBodyJson, const string& fieldName, string& errorMessage) const
        {
            if(!requestBodyJson.has_field(U(fieldName)))
            {
                errorMessage += fieldName + " is absent;";
            }
            else if(requestBodyJson[U(fieldName)].as_string().empty())
            {
                errorMessage += fieldName + " is empty;";
            }
        }

        static void validateAmountContractsPrice(json::value& requestBodyJson, std::string& errorMessage) {
            if(requestBodyJson.has_field(U("amount")) && requestBodyJson.has_field(U("contracts"))) {
                double amount = requestBodyJson[U("amount")].as_double();
                double contracts = requestBodyJson[U("contracts")].as_double();
                if(amount <= 0 || contracts <= 0 || amount != contracts) {
                    errorMessage += "amount and contracts must be greater than 0 and equal; ";
                }
            } 
            else if (requestBodyJson.has_field(U("amount"))) {
                double amount = requestBodyJson[U("amount")].as_double();
                if(amount <= 0) {
                    errorMessage += "amount must be greater than 0; ";
                }
            } 
            else if(requestBodyJson.has_field(U("contracts"))) {
                double contracts = requestBodyJson[U("contracts")].as_double();
                if(contracts <= 0) {
                    errorMessage += "contracts must be greater than 0; ";
                }
            }
        }
};