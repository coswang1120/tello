#include <tello/tello.hpp>
#include <tello/response.hpp>
#include <tello/command.hpp>
#include "response_factory.hpp"
#include <tello/connection/network.hpp>

using tello::Response;
using tello::ConnectionData;
using tello::Logger;
using tello::Status;

unordered_map<ip_address, const tello::Tello*> tello::Tello::_telloMapping;
std::shared_mutex tello::Tello::_telloMappingMutex;

tello::Tello::Tello(ip_address telloIp) : _clientaddr(sockaddrOf(telloIp)),
                                          _statusHandler(nullptr){
    _telloMappingMutex.lock();
    _telloMapping[telloIp] = this;
    _telloMappingMutex.unlock();
}

tello::Tello::~Tello() {
    _telloMappingMutex.lock();
    _telloMapping.erase(_clientaddr.sin_addr.s_addr);
    _telloMappingMutex.unlock();
}

void tello::Tello::setStatusHandler(status_handler statusHandler) {
    this->_statusHandler = statusHandler;
}

unique_ptr<Response> tello::Tello::exec(const Command& command) {
    return tello::Network::exec(command, *this);
}

sockaddr_in tello::Tello::sockaddrOf(ip_address telloIp) {
    sockaddr_in cliaddr{};
    memset(&cliaddr, 0, sizeof(cliaddr));

    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = telloIp;

    return cliaddr;
}