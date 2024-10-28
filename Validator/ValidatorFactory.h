#pragma once
#include "../utils.h"
#include "RequestValidator.h"
#include "ValidatorProvider.h"

enum class Action {
    AUTH,
    CREATE_ORDER,
    CANCEL_ORDER,
    EDIT_ORDER,
    GET_ORDERBOOK,
    GET_POSITIONS,
    UNKNOWN
};

Action stringToAction(const std::string& action) {
    static const std::unordered_map<std::string, Action> actionMap = {
        {"auth", Action::AUTH},
        {"create", Action::CREATE_ORDER},
        {"cancel", Action::CANCEL_ORDER},
        {"edit", Action::EDIT_ORDER},
        {"getOrderBook", Action::GET_ORDERBOOK},
        {"getPositions", Action::GET_POSITIONS}
    };

    auto it = actionMap.find(action);
    return (it != actionMap.end()) ? it->second : Action::UNKNOWN;
}

class ValidatorFactory 
{
    public:
        static std::unique_ptr<IRequestValidator> CreateValidator(const string& action) {
            std::unique_ptr<IValidatorFactory> factory;
            switch(stringToAction(action))
            {
                case Action::AUTH:
                    factory = std::make_unique<AuthValidatorFactory>();
                    break;
                case Action::CREATE_ORDER:
                    factory = std::make_unique<CreateOrderValidatorFactory>();
                    break;
                case Action::EDIT_ORDER:
                    factory = std::make_unique<ModifyOrderValidatorFactory>();
                    break;
                case Action::CANCEL_ORDER:
                    factory = std::make_unique<CancelOrderValidatorFactory>();
                    break;
                case Action::GET_ORDERBOOK:
                    factory = std::make_unique<GetOrderbookValidatorFactory>();
                    break;
                case Action::GET_POSITIONS:
                    factory = std::make_unique<GetPositionsValidatorFactory>();
                    break;
                default:
                    return nullptr;
            }
            std::unique_ptr<IRequestValidator> validator = factory->CreateValidator();
            return validator;
        }
};