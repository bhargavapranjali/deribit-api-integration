
pplx::task<json::value>handleGetPositionsUtil(http_request request, json::value requestBodyJson, string access_token)
{
    uri_builder uri(U(testDeribitUrl+"/private/get_positions"));
    if(requestBodyJson.has_field(U("currency")))
        uri.append_query(U("currency"), requestBodyJson[U("currency")].as_string());
    if(requestBodyJson.has_field(U("kind")))
        uri.append_query(U("kind"), requestBodyJson[U("kind")].as_string());
    if(requestBodyJson.has_field(U("subaccount_id")))
        uri.append_query(U("subaccount_id"), requestBodyJson[U("subaccount_id")].as_string());

    return sendDeribitRequest(uri,access_token);
}

void handleGetPositions(http_request request)
{
    std::unique_ptr<IRequestValidator> requestValidator = ValidatorFactory::CreateValidator("getPositions");
    pplx::task<json::value> requestBody = request.extract_json();
    json::value requestBodyJson = requestBody.get();
    RequestValidationResult validationResult = requestValidator->validateRequest(requestBodyJson);
    if(!validationResult.isRequestValid)
    {
        request.reply(status_codes::BadRequest, validationResult.errorMessage);
    }

    http_client client(U("http://localhost:8080/auth"));
    client.request(methods::POST,U(""),requestBodyJson.serialize(),U("application/json"))
        .then([request,requestBodyJson](web::http::http_response response) {
            return response.extract_json();
        })
        .then([request,requestBodyJson](web::json::value json_response) {
            if(json_response.has_field(U("access_token"))) {
                string access_token = json_response[U("access_token")].as_string();
                return handleGetPositionsUtil(request,requestBodyJson,access_token);
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