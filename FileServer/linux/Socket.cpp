#include "pch.h"
#include "../Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class SocketImpl
{
public:
    int s;
};

Socket::Socket(Protocol proto) : pImpl(std::make_shared<SocketImpl>())
{
    if (proto == Protocol::TCP)
        pImpl->s = socket(AF_INET, SOCK_STREAM, 0);
    else
        pImpl->s = -1;
}

bool Socket::valid() const
{
    return pImpl->s != -1;
}

bool Socket::startUp()
{
    return true;
}

void Socket::shutDown()
{
}

bool Socket::bind(const char* ip, int port)
{
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(port);

    // Ensure port is released to OS as soon as application closes
    int one = 1;
    setsockopt(pImpl->s, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

    auto result = ::bind(pImpl->s, (struct sockaddr*)&sa, sizeof(sa));
    return result != -1;
}

bool Socket::listen()
{
    auto result = ::listen(pImpl->s, SOMAXCONN);
    return result != -1;
}

Socket Socket::accept(std::string* clientIP)
{
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size = sizeof(clientaddr);
    Socket client(Protocol::None);
    client.pImpl->s = ::accept(pImpl->s, (struct sockaddr *)&clientaddr, &clientaddr_size);
    if (valid() && clientIP != nullptr)
    {
        char* strAddress = inet_ntoa(clientaddr.sin_addr);
        *clientIP = strAddress;
    }
    return client;
}

bool Socket::close()
{
    auto result = ::close(pImpl->s);
    return result != -1;
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
