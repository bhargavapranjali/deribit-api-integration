#include "utils.h"
#include "Validator/Validator.h"
#include "Controllers/OrderRequestHandler.h"
#include "Controllers/AuthRequestHandler.h"
#include "Controllers/PositionRequestHandler.h"
#include "Controllers/OrderBookRequestHandler.h"

void handleRequest(http_request request) {
    auto path = uri::decode(request.relative_uri().path());

    if(request.method()!=methods::POST)
        request.reply(status_codes::NotFound, U("Not Found"));

    try {
        if(path == U("/auth"))
            handleAuth(request);
        else if(path == U("/order"))
            handleOrder(request, request.relative_uri().query());
        else if(path == U("/getOrderBook"))
            handleGetOrderBook(request);
        else if(path == U("/getPositions"))
            handleGetPositions(request);
        else
            request.reply(status_codes::NotFound, U("Url not Found"));
    }
    catch(const json::json_exception& exception)
    {
        request.reply(status_codes::InternalError, U(exception.what()));
    }
    catch(const std::exception& exception) {
        request.reply(status_codes::InternalError, U(exception.what()));
    }
    catch(...) {
        request.reply(status_codes::InternalError, U("An exception occurred at backend"));
    }
}

int main()
{
    uri_builder localAuthAdress(U(localhostUrl));
    auto address = localAuthAdress.to_uri().to_string();

    http_listener listener(address);
    listener.support(handleRequest);

    listener
        .open()
        .then([&listener](){
            cout<<"Listening to url: "<<listener.uri().to_string()<<"\n";
        })
        .wait();

    sleep(100);
    return 0;
}