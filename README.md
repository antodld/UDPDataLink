# UDPDataLink

## Overview

UDPDataLink is a repository that enables communication of generic class objects using the UDP protocol.

## Dependencies

This repository relies on the Boost library.

## Installation 

1. Create a 'build' directory at the root of the repository.

2. In the 'build' directory, run the following commands:

   ```shell
   cmake ..
   sudo make install

## CMake export

  ```shell
  target_link_libraries(PROJECT PRIVATE UDPDataLink)

