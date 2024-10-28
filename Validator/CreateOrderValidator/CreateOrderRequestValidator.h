class CreateOrderValidator final: public IRequestValidator
{
    public:
        RequestValidationResult validateRequest(json::value& requestBodyJson) const override
        {
            string errorMessage = "";
            this->addErrorIfFieldInvalid(requestBodyJson,"instrument_name",errorMessage);
            this->addErrorIfFieldInvalid(requestBodyJson,"price",errorMessage);

            this->validateAmountContractsPrice(requestBodyJson,errorMessage);

            json::value errorResponse;
            if(!errorMessage.empty()) {
                errorResponse[U("error_message")] = web::json::value::string(errorMessage);
            }

            return RequestValidationResult(errorMessage.empty(), errorResponse);
        }
};