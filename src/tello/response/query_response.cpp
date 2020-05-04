#include <tello/response/query_response.hpp>
#include <tello/logger/logger.hpp>

using tello::Logger;
using tello::LoggerType;

tello::QueryResponse::QueryResponse(const Status& status) : Response(status),
                                                              _value(-1) {}

int tello::QueryResponse::value() const {
    return _value;
}


shared_ptr<tello::QueryResponse> tello::QueryResponse::error() {
    return std::make_shared<QueryResponse>(Status::FAIL);
}

shared_ptr<tello::QueryResponse> tello::QueryResponse::empty() {
    return std::make_shared<QueryResponse>(Status::UNKNOWN);
}

void tello::QueryResponse::update(const string &value) {
    _value = convert(value);
    _status = _value >= 0 ? Status::OK : Status::FAIL;
    callSubscriber();
}

int tello::QueryResponse::QueryResponse::convert(const string& stringValue) {
    try {
        int value = std::stoi(stringValue);
        return value;
    }

    catch (const std::invalid_argument& ia) {
        Logger::get(LoggerType::COMMAND)->error(string("Cannot parse query - 'invalid arg' [{}]"), stringValue);
        return -1;
    }

    catch (const std::out_of_range& oor) {
        Logger::get(LoggerType::COMMAND)->error(string("Cannot parse query - 'out of range' [{}]"), stringValue);
        return -2;
    }

    catch (const std::exception& e)
    {
        Logger::get(LoggerType::COMMAND)->error(string("Cannot parse query - 'exception' [{}]"), stringValue);
        return -3;
    }
}