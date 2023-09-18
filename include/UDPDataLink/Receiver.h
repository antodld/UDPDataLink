#pragma once
#ifndef RECEIVER_H
#define RECEIVER_H
#include "Serialize.h"
#include <iostream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <errno.h>
#include <fcntl.h>
#include <csignal>

namespace UDPDataLink
{
template <typename T>
class Receiver
{

public:
    Receiver()
    {
        // receiver(server_IP_,server_port_);
    }

    void create(const int port)
    {
        server_port_ = port;
        // Create a UDP socket
        socket_ = socket(AF_INET, SOCK_DGRAM, 0);
        
        // specify the server address and port
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(server_port_);
        
        // bind the socket to the server address and port
        int res = bind(socket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

        connected_ = true;

    }

    int receive()
    {
        int res = update();
        return res;
    }
    
    int get(T & data)
    {
        try 
        {
            data = deserializeObject<T>(serializedData_).data;
        } 
        catch (const boost::archive::archive_exception& ex) 
        {
            std::cout << "Caught archive_exception: deserializeObject failed " << ex.what() << std::endl;
            std::cout << "Serialized Data are : " << serializedData_ << std::endl;
            return 1;
            // Handle the exception (e.g., log the error or perform recovery)
        }
        return 0;   
    }
    
    ~Receiver()
    {
        // close the socket
        std::cout << "CLOSED" << std::endl;
        close(socket_);
    }
    bool connected()
    {
        return connected_;
    }

private:

    int update()
    {
        if(connected_)
        {
            int res = 0;
            res = receiveSerializedObject();
            return res;
        }
        return 1;
    }

    int receiveSerializedObject()
    {
 
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);

        size_t dataSize;

        int flags = fcntl(socket_, F_GETFL);
        fcntl(socket_, F_SETFL, flags | O_NONBLOCK);

        int recvStatus = recvfrom(socket_, &dataSize, sizeof(dataSize), 0,
                                    (struct sockaddr*)&clientAddr, &clientAddrLen);

        // std::cout << "recvStatus : " << recvStatus << " dataSize : " << dataSize << std::endl;

        if (recvStatus <= 0) {
            // std::cout << "No new data available at the moment." << std::endl;
            // Return an empty object to indicate no new data.
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                return 1;
            }

            // Error occurred while receiving data.
            // You can handle the error accordingly.
            std::cerr << "Error in recvfrom: " << strerror(errno) << std::endl;

            // For this example, we'll return an empty object.
            return 1;
        } 

        if(dataSize > 1e8){return 3;}

        char buffer[dataSize + 1];
        recvfrom(socket_, buffer, dataSize, 0,
                (struct sockaddr*)&clientAddr, &clientAddrLen);
        buffer[dataSize] = '\0';
        std::string serializedData(buffer);
        serializedData_ = serializedData;

        return 0;

    }

    std::string serializedData_;
    int server_port_ = 12345;
    int socket_;
    bool connected_ = false;

};
} //namespace UDPDataLink;
#endif 