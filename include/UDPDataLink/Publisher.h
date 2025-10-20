#pragma once
#ifndef SENDER_H
#define SENDER_H
#include "Serialize.h"
#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <udp_server.h>

namespace UDPDataLink
{


template <typename T>
struct Publisher : public UDPServer {
public:
    Publisher(uint16_t port) : UDPServer(port) {}

    // Optionally handle incoming messages
    void reception_callback(const uint8_t* buffer, size_t size) override {
        std::cout << "Received message from client: " << std::string((const char*)buffer, size) << std::endl;
    }

    // Publish data to the lastx client that sent a message
    void publish(const std::string& message) {
        send_data(reinterpret_cast<const uint8_t*>(message.data()), message.size());
    }

    void update_data(const T & data)
    {
        SerializableClass<T> objToSend(data, "Hello, Server!");
        publish(serializeObject(objToSend));
    }
};

}//namespace UDPDataLink
#endif
