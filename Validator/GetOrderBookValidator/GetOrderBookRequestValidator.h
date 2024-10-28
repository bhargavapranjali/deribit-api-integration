static const std::set<int> allowedValues = {1, 5, 10, 20, 50, 100, 1000, 10000};

class GetOrderbookValidator final: public IRequestValidator
{
    private:
        static bool isInvalidDepth(int number) 
        {
            return allowedValues.find(number) == allowedValues.end();
        }
    
    public:
        RequestValidationResult validateRequest(json::value& requestBodyJson) const override
        {
            string errorMessage = "";
            this->addErrorIfFieldInvalid(requestBodyJson,"instrument_name",errorMessage);

            if(requestBodyJson.has_field(U("depth"))) 
            {
                int depth = requestBodyJson[U("depth")].as_integer();
                if(isInvalidDepth(depth))
                {
                    errorMessage+= "depth is invalid;";
                }
            }
            json::value errorResponse;
            if(!errorMessage.empty()) {
                errorResponse[U("error_message")] = web::json::value::string(errorMessage);
            }
            return RequestValidationResult(errorMessage.empty(), errorResponse);
        }
};