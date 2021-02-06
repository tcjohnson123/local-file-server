#include "pch.h"
#include "../Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

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
    pImpl->sa.sin_addr.s_addr = inet_addr(ip);
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
	char* strAddress = inet_ntoa(pImpl->sa.sin_addr);
	return strAddress;
}

int net::EndPoint::port() const
{
	return ntohs(pImpl->sa.sin_port);
}
	
//////////

class net::SocketImpl
{
public:
    int s;
};

net::Socket::Socket(Protocol proto) : pImpl(std::make_shared<SocketImpl>())
{
    if (proto == Protocol::TCP)
        pImpl->s = socket(AF_INET, SOCK_STREAM, 0);
    else if (proto == Protocol::UDP)
        pImpl->s = socket(AF_INET, SOCK_DGRAM, 0);
    else
        pImpl->s = -1;
}

bool net::Socket::valid() const
{
    return pImpl->s != -1;
}

bool net::Socket::startUp()
{
    return true;
}

void net::Socket::shutDown()
{
}

bool net::Socket::bind(const EndPoint& endPoint)
{
    // Ensure port is released to OS as soon as application closes
    int one = 1;
    setsockopt(pImpl->s, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(one));

    auto result = ::bind(pImpl->s, (struct sockaddr*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
    return result != -1;
}

bool net::Socket::listen()
{
    auto result = ::listen(pImpl->s, SOMAXCONN);
    return result != -1;
}

net::Socket net::Socket::accept(EndPoint* clientIP)
{
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size = sizeof(clientaddr);
    Socket client(Protocol::None);
    client.pImpl->s = ::accept(pImpl->s, (struct sockaddr *)&clientaddr, &clientaddr_size);
    if (valid() && clientIP != nullptr)
    {
        clientIP->pImpl->sa = clientaddr;
    }
    return client;
}

bool net::Socket::close()
{
    auto result = ::close(pImpl->s);
    return result != -1;
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
    return ::sendto(pImpl->s, buf, len, flags, (struct sockaddr*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
}

int net::Socket::receiveFrom(char* buf, int len, EndPoint* sourceAddress)
{
    int flags = 0;
    socklen_t fromLen = (socklen_t)sizeof(sourceAddress->pImpl->sa);
    return ::recvfrom(pImpl->s, buf, len, flags, (struct sockaddr*)&sourceAddress->pImpl->sa, &fromLen);
}

net::EndPoint net::Socket::endPoint() const
{
    net::EndPoint endPoint;
    socklen_t nameLen = (int)sizeof(endPoint.pImpl->sa);
    //int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    ::getsockname(pImpl->s, (struct sockaddr*)&endPoint.pImpl->sa, &nameLen);
    return endPoint;
}

std::vector<std::string> net::Socket::getHostByName(const char* hostName)
{
    std::vector<std::string> hosts;
    struct hostent* remoteHost = ::gethostbyname(hostName);
    
    int i = 0;
    struct in_addr addr;
    if (remoteHost->h_addrtype == AF_INET)
    {
		while (remoteHost->h_addr_list[i] != 0) 
		{
			addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
			hosts.push_back(inet_ntoa(addr));
		}
	}
    
    return hosts;
}

bool net::Socket::connect(const EndPoint& endPoint)
{
    int rc = ::connect(pImpl->s, (struct sockaddr*)&endPoint.pImpl->sa, sizeof(endPoint.pImpl->sa));
    return (rc == 0);
}
