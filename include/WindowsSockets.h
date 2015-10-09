#ifndef __linux__
#pragma once
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define _WIN32_WINNT 0x501

#include <iostream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <stdio.h>

class Sockets
{
public:
    Sockets(std::string address, std::string port);
    virtual ~Sockets();
    bool Connect();
    void Disconnect();
    int Read(char* buffer, int size);
    int Send(char* data, int size);
private:
    bool connected;
    SOCKET socketfd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
};

#endif