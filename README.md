# Run
```
mkdir -p ./build
cd ./build
cmake ..
cmake --build . -j $(nproc)
```

# Notes

Network - Lightweight client
INetworkBus - Transport protocol interface
NetworkBus - Simple implementation of INetworkBus (see set_server method)
User - receives messages;
NetworkUser - Wrapper for sake of SDK API simplification