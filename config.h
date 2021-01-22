#ifndef config_h
#define config_h

// Node
#define NODE_ID "99"
#define NODE_NAME "Watcher"

// Power management
#define PIN_BAT 35
#define PIN_USB 34
#define VOLTAGE_COMPENSATION 1.086

// Network
#define SETUP_PASSWORD "potados90809"

// Socket
#define SOCKET_SERVER_HOST "10.0.1.10"
#define SOCKET_SERVER_PORT 8080
#define SOCKET_SERVER_PATH "/socket.io/?EIO=4&nodeId=" NODE_ID "&nodeName=" NODE_NAME
#define SOCKET_NAMESPACE "/node"
#define HEARTBEAT_INTERVAL 5000

#endif /* config_h */