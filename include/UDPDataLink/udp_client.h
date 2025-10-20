/*      File: udp_client.h
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
/**
 */
/**
 * @file pid/udp_client.h
 * @author Benjamin Navarro
 * @author Robin Passama
 * @brief header for udp-client library
 * @date 2022-2024
 * @ingroup pid-udp-client
 *
 * @defgroup pid-udp-client pid-udp-client: a UDP client
 * @details The library provide an object to simply implement a UDP client
 * @example example_udp_client.cpp
 */
#pragma once
#include <boost/asio.hpp>
#include <vector>
#include <thread>
/**
 * @brief Object for implementing a UDP client
 * @see UDPServer
 * @example example_udp_client.cpp
 */
class UDPClient {
public:
    /**
     * @brief Construct a new UDPClient object
     * @details no connection to server is performed
     *
     */
    UDPClient();
    /**
     * @brief Construct a new UDPClient object and immediately connect to server
     * @details there is no real connection in UDP, this call simply resolve the
     * endpoint to communicate with
     * @param [in] server_ip the IP adress of the server to connect with
     * @param [in] server_port the string representing the port number of the
     * server
     * @param [in] local_port the string representing the local port number of
     * the client
     * @param [in] max_packet_size maximum size for packets exchanged
     */
    UDPClient(const std::string& server_ip, const std::string& server_port,
              const std::string& local_port, size_t max_packet_size = 1024);

    /**
     * @brief Construct a new UDPClient object and immediately connect to server
     * @details there is no real connection in UDP, this call simply resolve the
     * endpoint to communicate with
     * @param [in] server_ip the IP adress of the server to connect with
     * @param [in] server_port the port number of the server
     * @param [in] local_port the local port number of the client
     * @param [in] max_packet_size maximum size for packets exchanged
     */
    UDPClient(const std::string& server_ip, uint16_t server_port,
              uint16_t local_port, size_t max_packet_size = 1024);
    /**
     * @brief Destroy the UDPClient object
     *
     */
    virtual ~UDPClient() = default;
    /**
     * @brief connect to a server
     * @details there is no real connection in UDP, this call simply resolve the
     * endpoint to communicate with
     * @param [in] server_ip the IP adress of the server to connect with
     * @param [in] server_port the string representing the port number of the
     * server
     * @param [in] local_port the string representing the local port number of
     * the client
     * @param [in] max_packet_size maximum size for packets exchanged
     */
    void connect(const std::string& server_ip, const std::string& server_port,
                 const std::string& local_port, size_t max_packet_size = 1024);
    /**
     * @brief connect to a server
     * @details there is no real connection in UDP, this call simply resolve the
     * endpoint to communicate with
     * @param [in] server_ip the IP adress of the server to connect with
     * @param [in] server_port the string representing the port number of the
     * server
     * @param [in] local_port the string representing the local port number of
     * the client
     * @param [in] max_packet_size maximum size for packets exchanged
     */
    void connect(const std::string& server_ip, uint16_t server_port,
                 uint16_t local_port, size_t max_packet_size = 1024);
    /**
     * @brief make the client verbose
     * @param state if true the client will be verbose
     */
    void set_verbose(bool state);
    /**
     * @brief receive a message
     * @details this call is blocking until the reception_callback is called
     */
    void receive();
    /**
     * @brief start asynchronous message reception
     * @details once called the reception_callback will be called any time a
     * message is received
     */
    void start_reception();
    /**
     * @brief stop asynchronous message reception
     * @details once called the reception_callback will no more be called any
     * time a message is received
     */
    void stop_reception();
protected:
    /**
     * @brief callback called anytime a message is received
     *
     * @param [in] buffer the pointer to the buffer containing the message
     * received
     * @param [in] size size of the message received
     */
    virtual void reception_callback(const uint8_t* buffer, size_t size);
    /**
     * @brief send data to server
     *
     * @param[in] buffer the pointer to memory buffer containing data to send
     * @param[in] size size of the buffer in bytes
     */
    void send_data(const uint8_t* buffer, size_t size);
private:
    void start_receive();
    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred);
    void handle_send(const boost::system::error_code& error,
                     std::size_t bytes_transferred);
    boost::asio::io_service io_service_;
    std::thread run_thread_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_, server_endpoint_;
    std::vector<uint8_t> buffer_in_;
    bool verbose_;
};
