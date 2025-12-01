#pragma once
#include "mutex"

// Ip address type
typedef unsigned int IPAddr;
// TCP port type
typedef unsigned short TCPPort;
// number type, - number used in the mathematical problem
typedef unsigned long long number;
// LG = "Lock Guard"
typedef std::lock_guard<std::mutex> LG;
// socket identifier type
typedef int SOCKET;

// working communication TCP port
extern const TCPPort portInfo;
// TCP port for signalling liveliness to kubernetes
extern const TCPPort portLive;