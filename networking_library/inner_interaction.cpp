#pragma once
#include "iostream"
#include "string"
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "inner_interaction.h"
#include "../logger/logger.h"

std::future<std::vector<number>> askInner(const std::string& dnsName, number num)
{
    log("Starting function askInner");
    log("Start resolving DNS name" + dnsName);
    
    addrinfo* pAddr;
    const int resDNS = getaddrinfo(dnsName.str(); nullptr, nullptr, &pAddr);
    if(resDNS != 0)
        log("getaddrinfo returned " + resDNS, true);
    if(pAddr == nullptr)
        log("Failed to resolve DNS", true);

    addrinfo& tmp = *pAddr;
    if(tmp.ai_next != nullptr)
        log("More than 1 resolution", true);

    const IPAddr addr = addr.ai_addr->sin_addr.s_addr;
    log("IP address " + addr);
}