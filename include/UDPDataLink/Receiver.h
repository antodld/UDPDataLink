#pragma once
#ifndef RECEIVER_H
#  define RECEIVER_H
#  include <boost/archive/text_iarchive.hpp>
#  include <boost/archive/text_oarchive.hpp>
#  include <boost/serialization/string.hpp>
#  include <boost/serialization/vector.hpp>
#  include "Serialize.h"
#  include <arpa/inet.h>
#  include <csignal>
#  include <errno.h>
#  include <fcntl.h>
#  include <iostream>
#  include <netinet/in.h>
#  include <sstream>
#  include <string>
#  include <sys/socket.h>
#  include <thread>
#  include <udp_client.h>
#  include <unistd.h>

namespace UDPDataLink
{

template<typename T>
struct Receiver : public UDPClient
{
  using UDPClient::UDPClient;

  void reception_callback(const uint8_t * buffer, size_t size) override
  {
    serializedData_ = std::string(reinterpret_cast<const char *>(buffer), size);
  }

  void send_data(const uint8_t * buffer, size_t size)
  {
    std::string msg = "hello world";
    UDPClient::send_data(reinterpret_cast<const uint8_t *>(msg.data()), msg.size());
  }

  int get(T & data)
  {
    if(serializedData_.size()) return 0;

    try
    {
      data = deserializeObject<T>(serializedData_).data;
    }
    catch(const boost::archive::archive_exception & ex)
    {
      std::cout << "Caught archive_exception: deserializeObject failed " << ex.what() << std::endl;
      std::cout << "Serialized Data are : " << serializedData_ << std::endl;
      return 1;
    }
    return 0;
  }

protected:
  std::string serializedData_;
};

} // namespace UDPDataLink
#endif
