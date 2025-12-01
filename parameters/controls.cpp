#include "netinet/in.h"
#include "controls.h"

static_assert(sizeof(IPAddr) == 4, "wrong type for IP address");

static_assert(sizeof(TCPPort) == 2, "wrong type for TCP port");
const TCPPort portInfo = 31000;
const TCPPort portLive = 31001;

static_assert(sizeof(number) == 8, "wrong type of number type");