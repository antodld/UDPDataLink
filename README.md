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

A data publisher of an object of class T can be initialized and used as such : 
```cpp
const char* serverIP = "127.0.0.1";
const int port = 1234
publisher = UDPDataLink::Publisher<T>();
publisher.create(serverIP, port);
//To send data
T data;
publihser.update_data(data);
```

## Receiver

A data receiver of an object of class T can be initialized and used as such : 
```cpp
const int port = 1234
receiver = UDPDataLink::Receiver<T>();
receiver.create(port);
//To Receive the first data in the buffer
T data;
int res = receiver.receive();
// res = 0 : OK ; 

//To convert the received data into the object
receiver.get(data);

```

# CMake export

  ```shell
  target_link_libraries(PROJECT PRIVATE UDPDataLink)

