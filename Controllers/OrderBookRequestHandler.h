void handleGetOrderBook(http_request request)
{
    std::unique_ptr<IRequestValidator> requestValidator = ValidatorFactory::CreateValidator("getOrderBook");
    pplx::task<json::value> requestBody = request.extract_json();
    json::value requestBodyJson = requestBody.get();
    RequestValidationResult validationResult = requestValidator->validateRequest(requestBodyJson);
    if(!validationResult.isRequestValid)
    {
        request.reply(status_codes::BadRequest, validationResult.errorMessage);
    }

    uri_builder uri(U(testDeribitUrl+"/public/get_order_book"));
    uri.append_query(U("instrument_name"), requestBodyJson[U("instrument_name")].as_string());
    if(requestBodyJson.has_field(U("depth")))
        uri.append_query(U("depth"), to_string(requestBodyJson[U("depth")].as_integer()));

    sendDeribitRequest(uri,"")
        .then([request](web::json::value json_response)
        {
            request.reply(status_codes::OK, json_response);
        })
        .wait();
}