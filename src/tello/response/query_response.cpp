#include <tello/response/query_response.hpp>
#include <tello/logger/logger_interface.hpp>

using tello::LoggerInterface;
using tello::LoggerType;

tello::QueryResponse::QueryResponse(const Status& status) : Response(status),
                                                              _value(-1) {}
tello::QueryResponse::QueryResponse(const string& value) : Response(Status::UNKNOWN),
                                                            _value(convert(value)) {
    _status = _value >= 0 ? Status::OK : Status::FAIL;
}

int tello::QueryResponse::value() const {
    return _value;
}

int tello::QueryResponse::QueryResponse::convert(const string& stringValue) {
    try {
        int value = std::stoi(stringValue);
        return value;
    }

    catch (const std::invalid_argument& ia) {
        LoggerInterface::error(LoggerType::COMMAND, string("Cannot parse query - 'invalid arg' [{}]"), stringValue);
        return -1;
    }

    catch (const std::out_of_range& oor) {
        LoggerInterface::error(LoggerType::COMMAND, string("Cannot parse query - 'out of range' [{}]"), stringValue);
        return -2;
    }

    catch (...)
    {
        LoggerInterface::error(LoggerType::COMMAND, string("Cannot parse query - 'exception' [{}]"), stringValue);
        return -3;
    }
}