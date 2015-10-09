#ifdef __linux__
#ifndef LINUXSOCKETS_H
#define LINUXSOCKETS_H
#include <string.h>
#include <cstring>      // Needed for memset
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>

class Sockets
{
    public:
        Sockets(std::string address, std::string port);
        virtual ~Sockets();
        bool Connect();
        void Disconnect();
        int Read(void* buffer, int size);
        int Send(void* data, int size);

        // -- Getters
        unsigned int GetCounter() { return m_Counter; }
        int GetStatus() { return status; }
        bool GetConnected() { return connected; }
        // -- Setters
        void SetCounter(int val) { m_Counter = val; }
    protected:
    private:
        bool connected;
        int status;
        int socketfd;
        struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
        struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
        unsigned int m_Counter;
};

#endif // LINUXSOCKETS_H
#endif
