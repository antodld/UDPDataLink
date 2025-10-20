UDPDataLink
==

UDPDataLink is a repository that enables communication of generic class objects using the UDP protocol.

# Dependencies

This repository relies on the Boost library.

# Installation

1. Create a 'build' directory at the root of the repository.

2. In the 'build' directory, run the following commands:

   ```shell
   cmake ..
   sudo make install

# Use

The object class supposed to be sent should be serializable by boost

## Publisher

A data publisher (UDP server)of an object of class T can be initialized and used as such :
```cpp
const int port = 1234
publisher = UDPDataLink::Publisher<T>(port);
publisher.start_reception(); // listen for incomming client messages
//To send data
T data;
publihser.update_data(data); // send data to last client
```

## Receiver

A data receiver of an object of class T can be initialized and used as such :
```cpp
const std::string server_ip = "192.168.1.15"; // ip of the publisher server
const std::string server_port = 1234; // same as the publisher above
const std::string local_port = 0; // local port for reception, leave as 0 to let the os automatically assign one
receiver = UDPDataLink::Receiver<T>(server_ip, server_port, local_port);
std::string msg = fmt::format("Hello from OpenVRPlugin");
receiver_->send_data(reinterpret_cast<const uint8_t *>(msg.data()), msg.size()); // Send a message to connect to the server
receiver.receive(); // start receiving data

//To convert the received data into the object
T data;
receiver.get(data);
```

# CMake export

```cmake
target_link_libraries(PROJECT PRIVATE UDPDataLink)
```
