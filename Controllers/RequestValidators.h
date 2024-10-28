// #pragma once

// struct RequestValidationResult
// {
//     bool isRequestValid;
//     web::json::value errorMessage;

//     RequestValidationResult(bool isRequestValid, json::value errorMessage): 
//         isRequestValid(isRequestValid), errorMessage(errorMessage){};
// };

// class IRequestValidator 
// {
//     public:
//         virtual RequestValidationResult validateRequest(json::value& requestBodyJson) const = 0;
//         virtual ~IRequestValidator() = default;
//     protected:
//         void addErrorIfFieldInvalid(json::value& requestBodyJson, const string& fieldName, string& errorMessage) const
//         {
//             if(!requestBodyJson.has_field(U(fieldName)))
//             {
//                 errorMessage += fieldName + " is absent;";
//             }
//             else if(requestBodyJson[U(fieldName)].as_string().empty())
//             {
//                 errorMessage += fieldName + " is empty;";
//             }
//         }

//         static void validateAmountContractsPrice(json::value& requestBodyJson, std::string& errorMessage) {
//             if(requestBodyJson.has_field(U("amount")) && requestBodyJson.has_field(U("contracts"))) {
//                 double amount = requestBodyJson[U("amount")].as_double();
//                 double contracts = requestBodyJson[U("contracts")].as_double();
//                 if(amount <= 0 || contracts <= 0 || amount != contracts) {
//                     errorMessage += "amount and contracts must be greater than 0 and equal; ";
//                 }
//             } 
//             else if (requestBodyJson.has_field(U("amount"))) {
//                 double amount = requestBodyJson[U("amount")].as_double();
//                 if(amount <= 0) {
//                     errorMessage += "amount must be greater than 0; ";
//                 }
//             } 
//             else if(requestBodyJson.has_field(U("contracts"))) {
//                 double contracts = requestBodyJson[U("contracts")].as_double();
//                 if(contracts <= 0) {
//                     errorMessage += "contracts must be greater than 0; ";
//                 }
//             }
//         }
// };

// class AuthValidator final: public IRequestValidator
// {
//     RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//     {
//         string errorMessage = "";
//         this->addErrorIfFieldInvalid(requestBodyJson,"clientId",errorMessage);
//         this->addErrorIfFieldInvalid(requestBodyJson,"clientSecret",errorMessage);

//         json::value errorResponse;
//         if(!errorMessage.empty()) {
//             errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//         }
//         return RequestValidationResult(errorMessage.empty(), errorResponse);
//     }
// };

// class CreateOrderValidator final: public IRequestValidator
// {
//     public:
//         RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//         {
//             string errorMessage = "";
//             this->addErrorIfFieldInvalid(requestBodyJson,"instrument_name",errorMessage);
//             this->addErrorIfFieldInvalid(requestBodyJson,"price",errorMessage);

//             this->validateAmountContractsPrice(requestBodyJson,errorMessage);

//             json::value errorResponse;
//             if(!errorMessage.empty()) {
//                 errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//             }

//             return RequestValidationResult(errorMessage.empty(), errorResponse);
//         }
// };

// class ModifyOrderValidator final: public IRequestValidator
// {
//     RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//     {
//         string errorMessage = "";
//         this->addErrorIfFieldInvalid(requestBodyJson,"order_id",errorMessage);

//         this->validateAmountContractsPrice(requestBodyJson,errorMessage);

//         json::value errorResponse;
//         if(!errorMessage.empty()) {
//             errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//         }
//         return RequestValidationResult(errorMessage.empty(), errorResponse);
//     }
// };

// class CancelOrderValidator final: public IRequestValidator
// {
//     RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//     {
//         string errorMessage = "";
//         this->addErrorIfFieldInvalid(requestBodyJson,"order_id",errorMessage);

//         json::value errorResponse;
//         if(!errorMessage.empty()) {
//             errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//         }
//         return RequestValidationResult(errorMessage.empty(), errorResponse);
//     }
// };

// class GetPositionsValidator final: public IRequestValidator
// {
//     RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//     {
//         string errorMessage = "";
//         this->addErrorIfFieldInvalid(requestBodyJson,"order_id",errorMessage);

//         json::value errorResponse;
//         if(!errorMessage.empty()) {
//             errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//         }
//         return RequestValidationResult(errorMessage.empty(), errorResponse);
//     }
// };

// static const std::set<int> allowedValues = {1, 5, 10, 20, 50, 100, 1000, 10000};

// class GetOrderbookValidator final: public IRequestValidator
// {
//     private:
//         static bool isInvalidDepth(int number) 
//         {
//             return allowedValues.find(number) == allowedValues.end();
//         }
    
//     public:
//         RequestValidationResult validateRequest(json::value& requestBodyJson) const override
//         {
//             string errorMessage = "";
//             this->addErrorIfFieldInvalid(requestBodyJson,"instrument_name",errorMessage);

//             if(requestBodyJson.has_field(U("depth"))) 
//             {
//                 int depth = requestBodyJson[U("depth")].as_integer();
//                 if(isInvalidDepth(depth))
//                 {
//                     errorMessage+= "depth is invalid;";
//                 }
//             }
//             json::value errorResponse;
//             if(!errorMessage.empty()) {
//                 errorResponse[U("error_message")] = web::json::value::string(errorMessage);
//             }
//             return RequestValidationResult(errorMessage.empty(), errorResponse);
//         }
// };

// class IValidatorFactory
// {
//     public:
//         virtual std::unique_ptr<IRequestValidator> CreateValidator() const = 0;
//         virtual ~IValidatorFactory() = default;
// };

// class AuthValidatorFactory final: public IValidatorFactory
// {
//     std::unique_ptr<IRequestValidator> CreateValidator() const override 
//     {
//         return std::make_unique<AuthValidator>();
//     }
// };

// class CreateOrderValidatorFactory final: public IValidatorFactory
// {
//     std::unique_ptr<IRequestValidator> CreateValidator() const override 
//     {
//         return std::make_unique<CreateOrderValidator>();
//     }
// };

// class ModifyOrderValidatorFactory final: public IValidatorFactory
// {
//     std::unique_ptr<IRequestValidator> CreateValidator() const override 
//     {
//         return std::make_unique<ModifyOrderValidator>();
//     }
// };

// class CancelOrderValidatorFactory final: public IValidatorFactory
// {
//     std::unique_ptr<IRequestValidator> CreateValidator() const override 
//     {
//         return std::make_unique<CancelOrderValidator>();
//     }
// };

// class GetOrderbookValidatorFactory final: public IValidatorFactory
// {
//     std::unique_ptr<IRequestValidator> CreateValidator() const override 
//     {
//         return std::make_unique<GetOrderbookValidator>();
//     }
// };
// class ValidatorFactory 
// {
//     public:
//         static std::unique_ptr<IRequestValidator> CreateValidator(const string& action) {
//             std::unique_ptr<IValidatorFactory> factory;
//             switch(action)
//             {
//                 case "auth":
//                     factory = std::make_unique<AuthValidatorFactory>();
//                     break;
//                 case "create":
//                     factory = std::make_unique<CreateOrderValidatorFactory>();
//                     break;
//                 case "edit":
//                     factory = std::make_unique<ModifyOrderValidatorFactory>();
//                     break;
//                 case "cancel":
//                     factory = std::make_unique<CancelOrderValidatorFactory>();
//                     break;
//                 case "getOrderBook":
//                     factory = std::make_unique<GetOrderbookValidatorFactory>();
//                     break;
//                 case "getPositions":
//                     factory = std::make_unique<GetPositionsValidatorFactory>();
//                     break;
//                 default:
//                     return nullptr;
//             }
            
//             std::unique_ptr<IRequestValidator> validator = factory->CreateValidator();

//             return validator;
//         }
// };

