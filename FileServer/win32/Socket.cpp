#include "pch.h"
#include "../Socket.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

class SocketImpl
{
public:
    SOCKET s;
};

Socket::Socket(Protocol proto) : pImpl(std::make_shared<SocketImpl>())
{
    if (proto == Protocol::TCP)
        pImpl->s = socket(AF_INET, SOCK_STREAM, 0);
    else
        pImpl->s = INVALID_SOCKET;
}

bool Socket::valid() const
{
    return pImpl->s != INVALID_SOCKET;
}

bool Socket::startUp()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;
    else
        return true;
}

void Socket::shutDown()
{
    WSACleanup();
}

bool Socket::bind(const char* ip, int port)
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    //sa.sin_addr.s_addr = inet_addr(ip);
    InetPtonA(AF_INET, ip, &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);

    // Ensure port is released to OS as soon as application closes
    int one = 1;
    setsockopt(pImpl->s, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

    auto result = ::bind(pImpl->s, (struct sockaddr*)&sa, sizeof(sa));
    return result != SOCKET_ERROR;
}

bool Socket::listen()
{
    auto result = ::listen(pImpl->s, SOMAXCONN);
    return result != SOCKET_ERROR;
}

Socket Socket::accept(std::string* clientIP)
{
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    Socket client(Protocol::None);
    client.pImpl->s = ::accept(pImpl->s, (SOCKADDR*)&addr, &addrlen);
    if (valid() && clientIP != nullptr)
    {
        char strAddress[64];
        InetNtopA(addr.sin_family, &addr.sin_addr, strAddress, sizeof(strAddress));
        *clientIP = strAddress;
    }
    return client;
}

bool Socket::close()
{
    auto result = ::closesocket(pImpl->s);
    return result != SOCKET_ERROR;
}

int Socket::send(const char* buf, int len)
{
    int flags = 0;
    return ::send(pImpl->s, buf, len, flags);
}

int Socket::receive(char* buf, int len)
{
    int flags = 0;
    return ::recv(pImpl->s, buf, len, flags);
}
