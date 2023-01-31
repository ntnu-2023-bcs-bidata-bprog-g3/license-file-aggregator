# license-aggregator

## Dependencies install
* C++ compiler of your choice
* Crow
    * Via terminal:
        ```bash 
            $ curl -vLJO https://github.com/CrowCpp/Crow/releases/download/v1.0%2B5/crow-v1.0+5.deb
            $ sudo apt install ./crow-v1.0+5.deb // 
        ```
    * Via browser: [Download](https://github.com/CrowCpp/Crow/releases/tag/v1.0+5), then install.
* Asio development headers (For crow)
    ```
        $ sudo apt-get install -y libasio-dev
    ```
* Install boost (For crow)
    ```bash
        $ sudo apt-get install libboost-all-dev
    ```

## Setup cmake
```bash
    $ mkdir build
    $ cd build
    $ cmake ../
```

## Building with cmake

Inside build folder:
```bash
    $ make
```