#include "network.hpp"
#include <tello/tello.hpp>
#include "../native/network_interface_factory.hpp"

#define COMMAND_PORT 8889
#define STATUS_PORT 8890
#define VIDEO_PORT 11111

using tello::Response;
using tello::NetworkResponse;

ConnectionData tello::Network::_commandConnection{-1, {}};
ConnectionData tello::Network::_statusConnection = {-1, {}};
ConnectionData tello::Network::_videoConnection = {-1, {}};
std::shared_mutex tello::Network::_connectionMutex;
shared_ptr<NetworkInterface> tello::Network::networkInterface = tello::NetworkInterfaceFactory::build();
VideoAnalyzer tello::Network::_videoAnalyzer;
Threadpool tello::Network::_threadpool;
UdpCommandListener tello::Network::_commandListener{_commandConnection, networkInterface, _connectionMutex};
UdpListener<tello::Network::invokeStatusListener> tello::Network::_statusListener{
        _statusConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::STATUS};

UdpListener<tello::Network::invokeVideoListener> tello::Network::_videoListener {
    _videoConnection, networkInterface, tello::Tello::_telloMapping, tello::Tello::_telloMappingMutex,
        tello::Network::_connectionMutex, LoggerType::VIDEO};

bool tello::Network::connect() {
    _connectionMutex.lock_shared();
    optional<ConnectionData> command = Network::connectToPort(COMMAND_PORT, _commandConnection, LoggerType::COMMAND);
    optional<ConnectionData> status = Network::connectToPort(STATUS_PORT, _statusConnection, LoggerType::STATUS);
    optional<ConnectionData> video = Network::connectToPort(VIDEO_PORT, _videoConnection, LoggerType::VIDEO);
    _connectionMutex.unlock_shared();

    bool isConnected = command && status && video;

    if (command) {
        _connectionMutex.lock();
        _commandConnection = command.value();
        _connectionMutex.unlock();
        LoggerInterface::info(LoggerType::COMMAND, string("Command-Port connected"), "");
    } else {
        LoggerInterface::info(LoggerType::COMMAND, string("Command-Port not connected"), "");
    }

    if (status) {
        _connectionMutex.lock();
        _statusConnection = status.value();
        _connectionMutex.unlock();
        LoggerInterface::info(LoggerType::STATUS, string("Status-Port connected"), "");
    } else {
        LoggerInterface::info(LoggerType::STATUS, string("Status-Port not connected"), "");
    }

    if (video) {
        _connectionMutex.lock();
        _videoConnection = video.value();
        _connectionMutex.unlock();
        LoggerInterface::info(LoggerType::VIDEO, string("Video-Port connected"), "");
    } else {
        LoggerInterface::info(LoggerType::VIDEO, string("Video-Port not connected"), "");
    }

    return isConnected;
}

void tello::Network::disconnect() {
    _statusListener.stop();
    _videoListener.stop();
    _commandListener.stop();
    _threadpool.stop();

    _connectionMutex.lock();
    if (_commandConnection._fileDescriptor != -1) {
        disconnect(_commandConnection, LoggerType::COMMAND);
    }
    if (_statusConnection._fileDescriptor != -1) {
        disconnect(_statusConnection, LoggerType::STATUS);
    }
    if (_videoConnection._fileDescriptor != -1) {
        disconnect(_videoConnection, LoggerType::VIDEO);
    }
    _connectionMutex.unlock();
}

optional<ConnectionData>
tello::Network::connectToPort(unsigned short port, const ConnectionData& data, const LoggerType& loggerType) {
    if (data._fileDescriptor != -1) {
        LoggerInterface::warn(loggerType, string("Is already connected!"));
        return std::make_optional<ConnectionData>(data);
    }

    return networkInterface->connect(NetworkData{SIN_FAM::I_AF_INET, port, 0}, loggerType);
}

void tello::Network::disconnect(ConnectionData& connectionData, const LoggerType& loggerType) {
    bool disconnected = networkInterface->disconnect(connectionData._fileDescriptor);
    if (disconnected) {
        LoggerInterface::info(loggerType, string("Socket closed"), "");
        _commandConnection._fileDescriptor = -1;
    } else {
        LoggerInterface::error(loggerType, string("Socket not closed"), "");
    }
}

void tello::Network::invokeStatusListener(NetworkResponse& networkResponse, const tello::Tello* tello) {
    if (tello->_statusHandler != nullptr) {
        tello->_statusHandler(StatusResponse{networkResponse.response()});
    }
}

void tello::Network::invokeVideoListener(NetworkResponse& response, const Tello* tello) {
    ip_address ip = response._sender._ip;
    bool frameFull = _videoAnalyzer.append(response);
    if (frameFull) {
        unsigned char* frame = _videoAnalyzer.frame(ip);
        unsigned int length = _videoAnalyzer.length(ip);
        _videoAnalyzer.clean(ip);

        _threadpool.push([tello, frame, length](int id) {
            if (tello->_videoHandler != nullptr) {
                VideoResponse videoResponse{frame, length};
                tello->_videoHandler(videoResponse);
            }
            delete[] frame;
        });
    }
}