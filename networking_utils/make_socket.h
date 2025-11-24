#pragma once
#include "optional"
#include "../parameters/controls.h"

// returns socket listening to incoming work connections
std::optional<SOCKET> listenInfo();

// class managing Kubernetes interaction: readiness, liveliness, startup
class CInteractKuberentes
{
public:
    static void start();
    static void terminateLive();

private:
    static void informLive();

private:
    static bool m_live;
    static std::mutex m_mutLive;
};

SOCKET connectToService();