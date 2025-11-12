#pragma once
#include "string"
#include "../parameters/controls.h"

// This file contains logger interface. The logger is thread-safe: it can be safely called from 
// different threads concurrently

// Logs a message
void log(const std::string& s);

// Sets name for the current thread
void setThreadName(const std::string& name, bool bError = false);