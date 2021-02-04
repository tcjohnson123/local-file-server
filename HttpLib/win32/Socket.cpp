#include "pch.h"

#include "../Socket.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

static std::vector<WCHAR> wideString(const char* utf8String)
{
    int cc = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);
    std::vector<WCHAR> data(cc);
    MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, &data[0], cc);
    return data;
}

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
    : pImpl(std::make_unique<EndPointImpl>())
{
    pImpl->sa.sin_family = AF_INET;
    //sa.sin_addr.s_addr = inet_addr(ip);
    InetPtonA(AF_INET, ip, &pImpl->sa.sin_addr.s_addr);
    pImpl->sa.sin_port = htons(port);
}

net::EndPoint::EndPoint(const EndPoint& other)
    : pImpl(std::make_unique<EndPointImpl>())
{
    pImpl->sa = other.pImpl->sa;
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

int net::Socket::receiveFrom(char* buf, int len, EndPoint* sourceAddress)
{
    int flags = 0;
    int fromLen = sizeof(sourceAddress->pImpl->sa);
    return ::recvfrom(pImpl->s, buf, len, flags, (SOCKADDR*)&sourceAddress->pImpl->sa, &fromLen);
}

net::EndPoint net::Socket::endPoint() const
{
    net::EndPoint endPoint;
    int nameLen = (int)sizeof(endPoint.pImpl->sa);
    ::getsockname(pImpl->s, (SOCKADDR*)&endPoint.pImpl->sa, &nameLen);
    return endPoint;
}

std::vector<std::string> net::Socket::getHostByName(const char* hostName)
{
    std::vector<std::string> hosts;

    DWORD dwRetval;
    struct addrinfo* result = NULL;
    struct addrinfo hints;
    struct addrinfo* ptr = nullptr;
    struct sockaddr_in* sockaddr_ipv4;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    dwRetval = ::getaddrinfo(hostName, "http", &hints, &result);
    if (dwRetval != 0)
        return hosts;

    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
    {
        if (ptr->ai_family == AF_INET)
        {
            sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
            char strAddress[64];
            InetNtopA(AF_INET, &sockaddr_ipv4->sin_addr, strAddress, sizeof(strAddress));
            hosts.push_back(strAddress);
        }
    }

    return hosts;
}

bool net::Socket::connect(const EndPoint& endPoint)
{
    int rc = ::connect(pImpl->s, (struct sockaddr*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
    return (rc == 0);
}
