#pragma once

#include <iostream>
#include <unistd.h>
#include <cpprest/json.h>
#include <cpprest/http_client.h>
#include <hiredis/hiredis.h>
#include <cpprest/http_listener.h>
#include <chrono>
#include <ctime>
#include <set>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::experimental::listener;
using namespace std;
using namespace utility;
using namespace std::chrono;

const string testDeribitUrl = "https://test.deribit.com/api/v2";
const string localhostUrl = "http://localhost:8080";

pplx::task<json::value> sendDeribitRequest(uri_builder uri, string access_token)
{
    http_client client(uri.to_uri());
    http_request request(methods::GET);

    if(access_token != "") request.headers().add(U("Authorization"), U("Bearer " + access_token));
    request.headers().add(U("Content-Type"), U("application/json"));

    return client.request(request)
        .then([uri](web::http::http_response response) {
            cout<<"utils::sendDeribitRequest Uri: "<<uri.to_uri().to_string()<<"\n";
            cout<<"utils::sendDeribitRequest Response code: "<<response.status_code()<<"\n";
            return response.extract_json();
        });
}
