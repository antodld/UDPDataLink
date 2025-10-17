/*      File: udp_server.cpp
 *       This file is part of the program pid-network-utilities
 *       Program description : A package providing libraries to standardize and
 * ease the implementation of network protocol. Copyright (C) 2016-2021 -
 * Benjamin Navarro (LIRMM/CNRS) Robin Passama (CNRS/LIRMM). All Right reserved.
 *
 *       This software is free software: you can redistribute it and/or modify
 *       it under the terms of the CeCILL-C license as published by
 *       the CEA CNRS INRIA, either version 1
 *       of the License, or (at your option) any later version.
 *       This software is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *       CeCILL-C License for more details.
 *
 *       You should have received a copy of the CeCILL-C License
 *       along with this software. If not, it can be found on the official
 * website of the CeCILL licenses family (http://www.cecill.info/index.en.html).
 */
#include "udp_server.h"
#include <iostream>
using namespace boost;
using boost::asio::ip::udp;
UDPServer::UDPServer() : socket_(io_service_), verbose_(false) {
}
UDPServer::UDPServer(uint16_t port, size_t max_packet_size) : UDPServer() {
    connect(port, max_packet_size);
}
void UDPServer::connect(uint16_t port, size_t max_packet_size) {
    buffer_in_.resize(max_packet_size);
    socket_ = udp::socket(io_service_, udp::endpoint(udp::v4(), port));
}
void UDPServer::set_verbose(bool state) {
    verbose_ = state;
}
void UDPServer::set_Verbose(bool state) {
    set_verbose(state);
}
void UDPServer::set_Remote_Endpoint(const std::string& remote_ip,
                                    uint16_t remote_port) {
    boost::system::error_code ec;
    auto ip_address = boost::asio::ip::address::from_string(remote_ip, ec);
    if (ec) {
        throw std::runtime_error("Invalid IP address: " + remote_ip);
    }
    remote_endpoint_ = udp::endpoint(ip_address, remote_port);
}
void UDPServer::receive() {
    io_service_.reset();
    start_receive();
    io_service_.run();
}
void UDPServer::start_Server() {
    receive();
}
void UDPServer::start_reception() {

    io_service_.reset();
    start_receive();
    run_thread_ = std::thread([this] { io_service_.run(); });
}
void UDPServer::start_Server_Thread() {
    start_reception();
}
void UDPServer::stop_reception() {
    io_service_.stop();
    run_thread_.join();
}
void UDPServer::stop_Server() {
    stop_reception();
}
void UDPServer::start_receive() {
    if (verbose_)
        std::cout << "Start listening on " << remote_endpoint_ << std::endl;
    socket_.async_receive_from(boost::asio::buffer(buffer_in_, buffer_in_.size()),
                               remote_endpoint_,
                               [this](auto error, auto bytes_transferred) {
                                   handle_receive(error, bytes_transferred);
                               });
}
void UDPServer::handle_receive(const boost::system::error_code& error,
                               std::size_t bytes_transferred) {
    if (!error) {
        if (verbose_)
            std::cout << "Message received (" << bytes_transferred
                      << " bytes) from " << remote_endpoint_ << std::endl;
        reception_callback(static_cast<const uint8_t*>(buffer_in_.data()),
                           bytes_transferred);
    } else {
        if (verbose_)
            std::cerr << "Error while receiving a message : " << error
                      << std::endl;
    }
    start_receive();
}
void UDPServer::handle_send(const boost::system::error_code& error,
                            std::size_t bytes_transferred) {
    if (verbose_) {
        if (!error)
            std::cout << "Message sent (" << bytes_transferred << " bytes) to "
                      << remote_endpoint_ << std::endl;
        else
            std::cerr << "Error while sending the response" << std::endl;
    }
}
void UDPServer::reception_callback(const uint8_t* buffer, size_t size) {
    send_data(buffer, size);
}
void UDPServer::reception_Callback(const uint8_t* buffer, size_t size) {
    send_data(buffer, size);
}
void UDPServer::reception_Callback(uint8_t* buffer, size_t size) {
    send_data(buffer, size);
}

void UDPServer::send_data(const uint8_t* buffer, size_t size) {
    if (verbose_)
        std::cout << "Sending data to " << remote_endpoint_ << std::endl;
    socket_.async_send_to(boost::asio::buffer(buffer, size), remote_endpoint_,
                          [this](auto error, auto bytes_transferred) {
                              handle_send(error, bytes_transferred);
                          });
}
void UDPServer::send_Data(const uint8_t* buffer, size_t size) {
    send_data(buffer, size);
}
