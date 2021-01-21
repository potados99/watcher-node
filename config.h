#ifndef config_h
#define config_h

// Power management
#define PIN_BAT 35
#define PIN_USB 34
#define VOLTAGE_COMPENSATION 1.086

// Network
#define NODE_NAME "Watcher"
#define SETUP_PASSWORD "potados90809"

// Socket
#define SOCKET_SERVER_HOST "10.0.1.10"
#define SOCKET_SERVER_PORT 8080
#define SOCKET_SERVER_PATH "/socket"
#define HEARTBEAT_INTERVAL 5

#endif /* config_h */