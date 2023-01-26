# license-aggregator

## Install openSSL

```
    sudo apt install libssl-dev
```

## Compile & Run

Compile the program with the library directory.
```
    g++ main.cpp -L{Path to openSSL} -lcrypto
```

Run by simply running the exe.
```
    ./a.out
```


