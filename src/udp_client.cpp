/*      File: udp_client.cpp
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
#include "udp_client.h"
#include <cstdlib>
#include <iostream>
using namespace boost;
using boost::asio::ip::udp;
UDPClient::UDPClient() : socket_(io_service_), verbose_(false) {}
UDPClient::UDPClient(const std::string & server_ip,
                     const std::string & server_port,
                     const std::string & local_port,
                     size_t max_packet_size)
: UDPClient()
{
  connect(server_ip, server_port, local_port, max_packet_size);
}
UDPClient::UDPClient(const std::string & server_ip, uint16_t server_port, uint16_t local_port, size_t max_packet_size)
: UDPClient(server_ip, std::to_string(server_port), std::to_string(local_port), max_packet_size)
{
}
void UDPClient::connect(const std::string & server_ip,
                        const std::string & server_port,
                        const std::string & local_port,
                        size_t max_packet_size)
{
  buffer_in_.resize(max_packet_size);
  socket_ = udp::socket(io_service_, udp::endpoint(udp::v4(), static_cast<uint16_t>(std::atoi(local_port.c_str()))));
  udp::resolver resolver(io_service_);
  udp::resolver::query query(udp::v4(), server_ip, server_port);
  server_endpoint_ = *resolver.resolve(query);
}
void UDPClient::connect(const std::string & server_ip,
                        uint16_t server_port,
                        uint16_t local_port,
                        size_t max_packet_size)
{
  buffer_in_.resize(max_packet_size);
  socket_ = udp::socket(io_service_, udp::endpoint(udp::v4(), local_port));
  udp::resolver resolver(io_service_);
  udp::resolver::query query(udp::v4(), server_ip, std::to_string(server_port));
  server_endpoint_ = *resolver.resolve(query);
}

void UDPClient::set_verbose(bool state)
{
  verbose_ = state;
}
void UDPClient::receive()
{
  io_service_.reset();
  start_receive();
  io_service_.run();
}
void UDPClient::start_reception()
{
  io_service_.reset();
  start_receive();
  run_thread_ = std::thread([this] { io_service_.run(); });
}
void UDPClient::stop_reception()
{
  io_service_.stop();
  run_thread_.join();
}
void UDPClient::start_receive()
{
  if(verbose_) std::cout << "Start listening on " << remote_endpoint_ << std::endl;
  socket_.async_receive_from(boost::asio::buffer(buffer_in_, buffer_in_.size()), remote_endpoint_,
                             [this](auto error, auto bytes_transferred) { handle_receive(error, bytes_transferred); });
}
void UDPClient::handle_receive(const boost::system::error_code & error, std::size_t bytes_transferred)
{
  if(!error)
  {
    if(bytes_transferred == buffer_in_.size())
    {
      auto newSize = buffer_in_.size() * 2;
      if(verbose_)
      {
        std::cout << "Warning: receive buffer was too small to handle message, doubling size from " << buffer_in_.size()
                  << " bytes to " << newSize << std::endl;
      }
      buffer_in_.resize(newSize);
    }
    else
    {
      if(verbose_)
        std::cout << "Message received (" << bytes_transferred << " bytes) from " << remote_endpoint_ << std::endl;
      reception_callback(static_cast<const uint8_t *>(buffer_in_.data()), bytes_transferred);
    }
  }
  else
  {
    if(verbose_) std::cerr << "Error while receiving a message : " << error << std::endl;
  }
  start_receive();
}
void UDPClient::handle_send(const boost::system::error_code & error, std::size_t bytes_transferred)
{
  if(verbose_)
  {
    if(!error)
      std::cout << "Message sent (" << bytes_transferred << " bytes) to "

                << server_endpoint_ << std::endl;
    else
      std::cerr << "Error while sending the response" << std::endl;
  }
}
void UDPClient::reception_callback(const uint8_t * buffer, size_t size)
{
  (void)buffer;
  (void)size;
}
void UDPClient::send_data(const uint8_t * buffer, size_t size)
{
  if(verbose_) std::cout << "Sending data to " << server_endpoint_ << std::endl;
  socket_.async_send_to(boost::asio::buffer(buffer, size), server_endpoint_,
                        [this](auto error, auto bytes_transferred) { handle_send(error, bytes_transferred); });
}
