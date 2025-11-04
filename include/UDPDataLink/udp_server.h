/*      File: udp_server.h
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
 * @file pid/udp_server.h
 * @author Benjamin Navarro
 * @author Robin Passama
 * @brief header for udp-server library
 * @date 2022-2024
 * @ingroup pid-udp-server
 *
 * @defgroup pid-udp-server pid-udp-server: a simple UDP server
 * @details The library provide an object to simply implement a UDP server
 * @example example_udp_server.cpp
 */
#pragma once
#include <boost/asio.hpp>
#include <cstdlib>
#include <iostream>
#include <list>
#include <thread>
#include <vector>
/**
 * @brief Object for implementing a UDP server
 * @see UDPClient
 * @example example_udp_server.cpp
 */
class UDPServer
{
public:
  /**
   * @brief Construct a new UDPServer object
   *
   */
  UDPServer();
  /**
   * @brief Construct a new UDPServer object and immediately connect
   * @see connect()
   * @param port port of the server
   * @param max_packet_size the maximum size for UDP messages exchanged
   */
  UDPServer(uint16_t port, size_t max_packet_size = 1024);
  /**
   * @brief Destroy the UDPServer object
   *
   */
  virtual ~UDPServer() = default;
  /**
   * @brief connect to a client
   * @details there is no real connection in UDP, this call simply resolve the

   * endpoint to communicate with
   * @param [in] port the IP adress of the server to connect with
   * @param [in] max_packet_size maximum size for packets exchanged
   */
  void connect(uint16_t port, size_t max_packet_size = 1024);
  /**
   * @brief make the server verbose
   * @param state if true the server will be verbose
   */
  void set_verbose(bool state);
  /**
   * @brief Set the remote endpoint for the client
   *
   * @param remote_ip IP adress of the client
   * @param remote_port port number of the client
   */
  void remote_endpoint(const std::string & remote_ip, uint16_t remote_port);
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
  virtual void reception_callback(const uint8_t * buffer, size_t size);
  /**
   * @brief send data to client
   *
   * @param [in] buffer the pointer to memory buffer containing data to send
   * @param [in] size size of the buffer in bytes
   */
  void send_data(const uint8_t * buffer, size_t size);

private:
  void start_receive();
  void handle_receive(const boost::system::error_code & error, std::size_t bytes_transferred);
  void handle_send(const boost::system::error_code & error, std::size_t bytes_transferred);
  boost::asio::io_service io_service_;
  std::thread run_thread_;
  boost::asio::ip::udp::socket socket_;
  struct ClientEndpoint
  {
    ClientEndpoint(const boost::asio::ip::udp::endpoint & ep, size_t default_packet_size = 0, bool verbose = false)
    : endpoint(ep)
    {
      buffer_out_.resize(default_packet_size);
    }

    void send_data(boost::asio::ip::udp::socket & socket_, const uint8_t * buffer, size_t size)
    {
      if(sending_)
      {
        std::cout << "A buffer is already being sent, skipping message" << std::endl;
      }
      if(size > buffer_out_.size())
      {
        if(verbose_)
        {
          std::cout << "Send buffer is too small, resizing to " << size << std::endl;
        }
        buffer_out_.resize(size);
      }
      buffer_out_.assign(buffer, buffer + size);
      if(verbose_) std::cout << "Sending data to " << endpoint << ", size: " << size << std::endl;
      sending_ = true;
      socket_.async_send_to(boost::asio::buffer(buffer_out_, size), endpoint,
                            [this](auto error, auto bytes_transferred) { handle_sent(error, bytes_transferred); });
    }

    void handle_sent(const boost::system::error_code & error, std::size_t bytes_transferred)
    {
      if(verbose_)
      {
        if(!error)
          std::cout << "Message sent (" << bytes_transferred << " bytes) " << std::endl;
        else
          std::cerr << "Error while sending the response" << std::endl;
      }
      sending_ = false;
    }

  protected:
    std::vector<uint8_t> buffer_out_;
    boost::asio::ip::udp::endpoint endpoint;
    bool sending_ = false;
    bool verbose_ = false;
  };

  std::list<ClientEndpoint> remote_endpoints_;
  boost::asio::ip::udp::endpoint new_client_endpoint_;
  std::vector<uint8_t> buffer_in_;
  bool verbose_;
};
