# License File Aggregator

## Oatpp
This repo was based on a starter project of oat++. 
See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)

## Overview

### Purpose
The `License File Aggregator (LFA)` is made to function as a functionality consumer. I.E the place where a license for a certain functionality is actually ran and used up. 

It has been designed for operating within an `Offline License Management System (olm)` and is assumed to be ran on proprietary hardware as to ensure its integrity. 

This repo was made to be used together with the `nms` and functions as a Proof of Concept toghether with it. This means certain functionality that would be required within a real `olm` system is either missing / changed for time or due to it having been proved in the `nms` already.

### Project layout

```
.
|-- CMakeLists.txt
|-- LICENSE
|-- README.md
|-- src
|   |-- App.cpp
|   |-- AppComponent.hpp
|   |-- client
|   |   `-- client.hpp
|   |-- controller
|   |   |-- Controller.cpp
|   |   `-- Controller.hpp
|   |-- dto
|   |   `-- DTOs.hpp
|   |-- error
|   |   |-- error.cpp
|   |   `-- error.hpp
|   |-- file
|   |   |-- fileHandler.cpp
|   |   `-- fileHandler.hpp
|   |-- shared.hpp
|   `-- ssl
|       |-- certificates.cpp
|       `-- certificates.hpp
|-- test
|   |-- ControllerTest.cpp
|   |-- ControllerTest.hpp
|   |-- app
|   |   |-- MyApiTestClient.hpp
|   |   `-- TestComponent.hpp
|   `-- tests.cpp
`-- utility
    `-- install-oatpp-modules.sh
```

---

### Build and Run

**Requires**   
- Fetch git submodule `oatpp-openssl`
```
$ git submodule update --init --recursive
```
- `oatpp` module installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

#### Using CMake

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./lfa-exe  # - run application.

```
the secret is '`secret`'
