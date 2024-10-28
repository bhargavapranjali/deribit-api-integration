pplx::task<json::value> handleOperation(http_request request, const string& operation, json::value requestBodyJson, string access_token)
{
    if(operation == "create")
    {
        uri_builder uri(U(testDeribitUrl+"/private/buy"));
        uri.append_query(U("instrument_name"), requestBodyJson[U("instrument_name")].as_string());
        if(requestBodyJson.has_field(U("amount")))
            uri.append_query(U("amount"), to_string(requestBodyJson[U("amount")].as_integer()));
        if(requestBodyJson.has_field(U("price")))
            uri.append_query(U("price"), requestBodyJson[U("price")].as_string());
        if(requestBodyJson.has_field(U("contracts")))
            uri.append_query(U("contracts"), to_string(requestBodyJson[U("contracts")].as_integer()));
        if(requestBodyJson.has_field(U("type")))
            uri.append_query(U("type"), requestBodyJson[U("type")].as_string());

        return sendDeribitRequest(uri,access_token);
    }
    else if(operation == "edit")
    {        
        uri_builder uri(U(testDeribitUrl+"/private/edit"));
        uri.append_query(U("order_id"), requestBodyJson[U("order_id")].as_string());
        if(requestBodyJson.has_field(U("amount")))
            uri.append_query(U("amount"), to_string(requestBodyJson[U("amount")].as_integer()));
        uri.append_query(U("price"), requestBodyJson[U("price")].as_string());
        uri.append_query(U("contracts"), to_string(requestBodyJson[U("contracts")].as_integer()));

        return sendDeribitRequest(uri,access_token);
    }
    else if(operation == "cancel")
    {
        uri_builder uri(U(testDeribitUrl+"/private/cancel"));
        uri.append_query(U("order_id"), requestBodyJson[U("order_id")].as_string());

        return sendDeribitRequest(uri,access_token);
    }
    return pplx::task<web::json::value>();
}

void handleOrder(http_request request, const utility::string_t& queryParameterString)
{
    http_client client(U("http://localhost:8080/auth"));

    pplx::task<json::value> requestBody = request.extract_json();
    json::value requestBodyJson = requestBody.get();

    auto queryParams = uri::split_query(queryParameterString);
    if(queryParams.find("action") == queryParams.end())
    {
        json::value errorJson;
        errorJson[U("errorMessage")] = json::value::string(U("Action query parameter is absent"));
        request.reply(status_codes::BadRequest,errorJson);
        return;
    }
    string action = queryParams[U("action")];

    std::unique_ptr<IRequestValidator> requestValidator = ValidatorFactory::CreateValidator(action);
    RequestValidationResult validationResult = requestValidator->validateRequest(requestBodyJson);
    if(!validationResult.isRequestValid)
    {
        request.reply(status_codes::BadRequest,validationResult.errorMessage);
        return;
    }

    client.request(methods::POST,U(""),requestBodyJson.serialize(),U("application/json"))
        .then([request,requestBodyJson,queryParameterString](web::http::http_response response) {
            return response.extract_json();
        })
        .then([request,requestBodyJson,action](web::json::value json_response) {
            if(json_response.has_field(U("access_token"))) {
                string access_token = json_response[U("access_token")].as_string();
                return handleOperation(request,action,requestBodyJson,access_token);
            } else {
                cout<<"OrderRequestHandler::handleOrder Access token is not available for given client credentials\n" << std::endl;
                request.reply(status_codes::BadRequest, json_response);
                return pplx::task_from_result(json_response);
            }
        })
        .then([request](web::json::value response){
            request.reply(status_codes::OK, response);
        })
        .wait();
}