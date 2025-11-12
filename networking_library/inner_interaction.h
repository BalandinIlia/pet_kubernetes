#pragma once
#include "vector"
#include "future"
#include "../parameters/controls.h"

std::future<std::vector<number>> askInner(const std::string& dnsName, number num);

number getInner(SOCKET sockId);