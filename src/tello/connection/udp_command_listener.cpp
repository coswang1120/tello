#include <tello/connection/udp_command_listener.hpp>
#include <tello/response.hpp>
#include <tello/response/query_response.hpp>

tello::UdpCommandListener::UdpCommandListener(const tello::ConnectionData& connectionData,
                                              shared_ptr<NetworkInterface> networkInterface,
                                              std::shared_mutex& connectionMutex)
        : _exitSignal(),
          _mapping(),
          _responseMutex(),
          _worker(thread(&UdpCommandListener::listen, std::ref(connectionData), networkInterface,
                         std::ref(connectionMutex), _exitSignal.get_future(), std::ref(_mapping),
                         std::ref(_responseMutex))) {}

tello::ResponseMapping::ResponseMapping(Response response, promise<Response>& prom, time_t insertDate) :
        _response(std::move(response)), _queryResponse(), _prom(std::move(prom)), _queryProm(),
        _insertDate(insertDate), _responseMappingType(ResponseMappingType::RESPONSE) {}

tello::ResponseMapping::ResponseMapping(QueryResponse response, promise<QueryResponse>& prom, time_t insertDate) :
        _response(), _queryResponse(std::move(response)), _prom(), _queryProm(std::move(prom)),
        _insertDate(insertDate), _responseMappingType(ResponseMappingType::QUERY_RESPONSE) {}

void tello::ResponseMapping::set_value(const string& value) {
    if (_responseMappingType == ResponseMappingType::RESPONSE) {
        _response.update(value);
    } else if (_responseMappingType == ResponseMappingType::QUERY_RESPONSE) {
        _queryResponse.update(value);
    }
    complete();
}

void tello::ResponseMapping::set_value(const Status& status) {
    if(_responseMappingType == ResponseMappingType::RESPONSE) {
        _response.update(status);
    } else if (_responseMappingType == ResponseMappingType::QUERY_RESPONSE) {
        _queryResponse.update(status);
    }
    complete();
}

void tello::ResponseMapping::complete() {
    if(_responseMappingType == ResponseMappingType::RESPONSE) {
        _prom.set_value(_response);
    } else if (_responseMappingType == ResponseMappingType::QUERY_RESPONSE) {
        _queryProm.set_value(_queryResponse);
    }
}

void tello::UdpCommandListener::stop()  {
    _exitSignal.set_value();
    _worker.join();
}

vector<shared_ptr<tello::ResponseMapping>> tello::UdpCommandListener::cleanMapping(
        unordered_map<ip_address, vector<shared_ptr<ResponseMapping>>>& mapping)  {
    time_t now = currentTime();
    vector<shared_ptr<ResponseMapping>> removed;
    for (auto entry = mapping.begin(); entry != mapping.end(); ++entry) {
        shared_ptr<ResponseMapping> response = entry->second.at(0);
        if (now - response->_insertDate >= 15) {
            removed.push_back(response);
            entry->second.erase(entry->second.begin());
        }

        if (entry->second.empty()) {
            mapping.erase(entry->first);
        }
    }
    return removed;
}

void tello::UdpCommandListener::listen(const tello::ConnectionData& connectionData,
                                       shared_ptr<NetworkInterface> networkInterface,
                                       std::shared_mutex& connectionMutex, future<void> exitListener,
                                       unordered_map<ip_address, vector<shared_ptr<ResponseMapping>>>& mapping,
                                       std::mutex& responseMutex)  {
    bool isFirstAccessToFileDescriptor = true;

    while (exitListener.wait_for(std::chrono::milliseconds(30)) == std::future_status::timeout) {
        connectionMutex.lock_shared();
        if (connectionData._fileDescriptor == -1) {
            connectionMutex.unlock_shared();
            continue;
        } else if (isFirstAccessToFileDescriptor) {
            Logger::get(LoggerType::COMMAND)->info(string("Start listen to port {0:d}"),
                                                   connectionData._networkData._port);
            isFirstAccessToFileDescriptor = false;
        }

        NetworkResponse networkResponse = networkInterface->read(connectionData._fileDescriptor);
        connectionMutex.unlock_shared();

        responseMutex.lock();
        ip_address senderIp = networkResponse._sender._ip;
        auto sender = mapping.find(senderIp);
        bool correctSender = sender != mapping.end();

        if (!correctSender) {
            Logger::get(LoggerType::COMMAND)->error(
                    string("Answer from wrong tello received {0:x}"), senderIp);
        } else {
            string answer = networkResponse._response;
            shared_ptr<ResponseMapping> response = sender->second.at(0);
            sender->second.erase(sender->second.begin());
            if (sender->second.empty()) {
                mapping.erase(sender->first);
            }
            response->set_value(answer);
        }

        vector<shared_ptr<ResponseMapping>> removed = cleanMapping(mapping);

        responseMutex.unlock();

        for(auto& response : removed) {
            response->set_value(Status::TIMEOUT);
        }
    }

    Logger::get(LoggerType::COMMAND)->info(string("Stop listen to port {0:d}"),
                                           connectionData._networkData._port);
}

time_t tello::UdpCommandListener::currentTime() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(now).count();
}