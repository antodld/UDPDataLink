#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

namespace UDPDataLink
{
template<typename T>
class SerializableClass
{
public:
  T data;
  std::string message;

  // Constructor
  SerializableClass(const T & data = T{}, const std::string & message = "") : data(data), message(message) {}

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & data;
    ar & message;
  }
};

template<typename T>
std::string serializeObject(const SerializableClass<T> & obj)
{
  std::ostringstream oss;
  boost::archive::text_oarchive archive(oss);
  archive << obj;
  return oss.str();
}

template<typename T>
SerializableClass<T> deserializeObject(const std::string & serializedData)
{
  std::istringstream iss(serializedData);
  boost::archive::text_iarchive archive(iss);
  SerializableClass<T> obj;
  archive >> obj;
  return obj;
}
} // namespace UDPDataLink
