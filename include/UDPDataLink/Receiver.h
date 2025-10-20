#pragma once

#include "Serialize.h"
#include <iostream>
#include <string>
#include <udp_client.h>

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

  bool get(T & data)
  {
    if(serializedData_.empty()) return false;

    try
    {
      data = deserializeObject<T>(serializedData_).data;
    }
    catch(const boost::archive::archive_exception & ex)
    {
      std::cout << "Caught archive_exception: deserializeObject failed " << ex.what() << std::endl;
      std::cout << "Serialized Data are : " << serializedData_ << std::endl;
      return false;
    }
    return true;
  }

protected:
  std::string serializedData_;
};

} // namespace UDPDataLink
