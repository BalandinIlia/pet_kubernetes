#pragma once
#include "../networking_utils/make_socket.h"

// This function serves a client with given id connected to given socket
void serveClient(SOCK&& sock, int idClient);