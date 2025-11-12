#pragma once
#include "../parameters/controls.h"

// returns socket listing to incoming work connections
SOCKET listenInfo();

// class managing Kubernetes interaction: readiness, liveliness, startup
class CInteractKuberentes
{
public:
    CInteractKuberentes();
    void setReady(bool bReady);

private:
    bool m_ready;
};