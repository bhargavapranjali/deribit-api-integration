redisContext* CreateRedisInstance()
{
    redisContext* context = redisConnect("127.0.0.1",6379);
    if(!context || context->err)
    {
        if(context) {
            cerr<<"AuthRequestHandler::CreateRedisInstance " << context->errstr << std::endl;
            redisFree(context);
        } else {
            std::cerr << "AuthRequestHandler::CreateRedisInstance: Can't allocate redis context" << std::endl;
        }
    }
    return context;
}

char* GetRedisCacheKey(redisContext* context, string key)
{
    redisReply* reply = (redisReply*)redisCommand(context, "GET %s", key.c_str());
    std::cout << "AuthRequestHandler::GetRedisCacheKey - Reply status: "<<reply->type << std::endl;
    if(reply->type == REDIS_REPLY_STRING){
        return reply->str;
    }
    freeReplyObject(reply);
    return NULL;
}

void SetRedisCacheKey(redisContext* context, std::string key, std::string value)
{
    redisReply* reply = (redisReply*) redisCommand(context, "SET %s %s", key.c_str(), value.c_str());
    std::cout << "AuthRequestHandler::SetRedisCacheKey - Reply status: "<<reply->type<<" SET: " << reply->str << std::endl;
    freeReplyObject(reply);
}

void handleAuth(http_request request)
{
    pplx::task<json::value> requestBody = request.extract_json();
    json::value requestBodyJson = requestBody.get();

    // Validating request
    std::unique_ptr<IRequestValidator>requestValidator = ValidatorFactory::CreateValidator("auth");
    RequestValidationResult validationResult = requestValidator->validateRequest(requestBodyJson);

    if(!validationResult.isRequestValid)
    {
        request.reply(status_codes::BadRequest, validationResult.errorMessage);
    }

    std::string clientId = requestBodyJson[U("clientId")].as_string();
    std::string clientSecret = requestBodyJson[U("clientSecret")].as_string();

    //Check if access token corresponding to client id and client secret exists

    redisContext* context = CreateRedisInstance();
    char* replyStr = GetRedisCacheKey(context, clientId+"_"+clientSecret);
    
    if(replyStr)
    {
        json::value jsonData = json::value::parse(replyStr);

        //if expires_in is less than current time return else call api and cache the access token and refresh token
        string access_token = jsonData[U("access_token")].as_string();
        int startTime = jsonData[U("startTime")].as_integer();
        int expires_in = jsonData[U("expires_in")].as_integer();

        auto current_time = system_clock::now();
        auto epoch_time = duration_cast<seconds>(current_time.time_since_epoch()).count();
        cout<<"AuthRequestHandler::handleAuth Is token expired? "<<(startTime+expires_in > epoch_time ? "false" : "true") << "\n";

        if(startTime+expires_in>epoch_time) {
            request.reply(status_codes::OK, jsonData);
            return;
        }
    }

    //Make a call to deribitTest
    uri_builder uri(U(testDeribitUrl+"/public/auth"));
    uri.append_query(U("client_id"), clientId);
    uri.append_query(U("client_secret"), clientSecret);
    uri.append_query(U("grant_type"), "client_credentials");

    http_request authRequest(methods::GET);
    http_client client(uri.to_uri());

    sendDeribitRequest(uri,"")
        .then([context,clientId,clientSecret,request](web::json::value json_response) {

            if(!json_response.has_field(U("result")))
                request.reply(status_codes::BadRequest, json_response);

            //Extract values to store in redis cache
            json::value result = json_response[U("result")];

            //Create json object to cache corresponding to key "{client_id}_{client_secret}"
            json::value valueData;
            valueData[U("access_token")] = result[U("access_token")];
            valueData[U("expires_in")] = result[U("expires_in")];

            // Get the current time as a UNIX timestamp
            auto current_time = system_clock::now();
            auto epoch_time = duration_cast<seconds>(current_time.time_since_epoch()).count();
            valueData[U("startTime")] = json::value::number(epoch_time);

            SetRedisCacheKey(context, clientId+"_"+clientSecret, valueData.serialize());
            request.reply(status_codes::OK, valueData);
        })
        .wait();

    request.reply(status_codes::InternalError, "");
}