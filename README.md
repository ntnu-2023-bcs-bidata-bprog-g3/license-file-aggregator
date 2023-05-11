# License File Aggregator
## Overview

### Purpose
The `License File Aggregator (LFA)` is made to function as a functionality consumer. I.E the place where a license for a certain functionality is actually ran and used up. 

It has been designed for operating within an `Offline License Management System (OLM)` and is assumed to be ran on proprietary hardware as to ensure its integrity. 

This repo was made to be used together with the `NMS` and functions as a Proof of Concept toghether with it. This means certain functionality that would be required within a real `OLM` system is either missing / changed for time or due to it having been proved in the `NMS` already.

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

## Build and Run

### Requiments
- Git submodule `oatpp-openssl`. Run following command to install.
    ```
    $ git submodule update --init --recursive
    ```
- `oatpp` module installed. You may run `utility/install-oatpp-modules.sh` 
script to install required oatpp modules.

### Build

```
$ mkdir build && cd build
$ cmake ..
$ make 
```

### Run
Inside the build folder.
```
$ ./lfa-exe  # - run application.
```
the secret is '`secret`' if using the default chain of trust.

When running the `LFA` it is important to enter the secret before sending  `NMS` requests. If the secret has not been given, the server is not ready and the `NMS` will remove it from its list of active `LFAs`. Should this happen, the `LFA` would need a reboot to be recognized by the `NMS`.

### Swapping certificates
If you are changing the chain of trust, all that needs to be swapped is the file '`./cert/external/root.cert`' with the new root certificate of the same name.

The contents of the '`./cert/local/`' folder is for HTTPS and can be swapped with new files of the same names if wanted. 

## Oatpp
This repo was based on a starter project of oat++. 
See more:

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
