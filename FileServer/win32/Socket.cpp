#include "pch.h"
#include "../Socket.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

class net::EndPointImpl
{
public:
    struct sockaddr_in sa;
};

net::EndPoint::EndPoint()
    : EndPoint("0.0.0.0", 0)
{
}

net::EndPoint::EndPoint(const char* ip, int port)
{
    pImpl->sa.sin_family = AF_INET;
    //sa.sin_addr.s_addr = inet_addr(ip);
    InetPtonA(AF_INET, ip, &pImpl->sa.sin_addr.s_addr);
    pImpl->sa.sin_port = htons(port);
}

net::EndPoint::~EndPoint()
{
}

std::string net::EndPoint::ipAddress() const
{
    char strAddress[64];
    InetNtopA(pImpl->sa.sin_family, &pImpl->sa.sin_addr, strAddress, sizeof(strAddress));
    return strAddress;
}

int net::EndPoint::port() const
{
    return ntohs(pImpl->sa.sin_port);
}

////////////////////////////////////

class net::SocketImpl
{
public:
    SOCKET s;
};

net::Socket::Socket(Protocol proto) : pImpl(std::make_shared<SocketImpl>())
{
    if (proto == Protocol::TCP)
        pImpl->s = socket(AF_INET, SOCK_STREAM, 0);
    else if (proto == Protocol::UDP)
        pImpl->s = socket(AF_INET, SOCK_DGRAM, 0);
    else
        pImpl->s = INVALID_SOCKET;
}

bool net::Socket::valid() const
{
    return pImpl->s != INVALID_SOCKET;
}

bool net::Socket::startUp()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return false;
    else
        return true;
}

void net::Socket::shutDown()
{
    WSACleanup();
}

bool net::Socket::bind(const EndPoint& endPoint)
{
    // Ensure port is released to OS as soon as application closes
    int one = 1;
    setsockopt(pImpl->s, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

    auto result = ::bind(pImpl->s, (struct sockaddr*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
    return result != SOCKET_ERROR;
}

bool net::Socket::listen()
{
    auto result = ::listen(pImpl->s, SOMAXCONN);
    return result != SOCKET_ERROR;
}

net::Socket net::Socket::accept(EndPoint* clientIP)
{
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    Socket client(Protocol::None);
    client.pImpl->s = ::accept(pImpl->s, (SOCKADDR*)&addr, &addrlen);
    if (valid() && clientIP != nullptr)
    {
        clientIP->pImpl->sa = addr;
    }
    return client;
}

bool net::Socket::close()
{
    auto result = ::closesocket(pImpl->s);
    return result != SOCKET_ERROR;
}

int net::Socket::send(const char* buf, int len)
{
    int flags = 0;
    return ::send(pImpl->s, buf, len, flags);
}

int net::Socket::receive(char* buf, int len)
{
    int flags = 0;
    return ::recv(pImpl->s, buf, len, flags);
}

int net::Socket::sendTo(const char* buf, int len, const EndPoint& endPoint)
{
    int flags = 0;
    return ::sendto(pImpl->s, buf, len, flags, (SOCKADDR*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
}
