#pragma once

#include "Stream.h"
#include <memory>
#include <string>

class SocketImpl;

enum class Protocol
{
	None,
	TCP
};

class Socket : public net::Stream
{
public:
	Socket(Protocol proto);

	static bool startUp();
	static void shutDown();
	bool valid() const;
	bool bind(const char* ip, int port);
	bool listen();
	bool close();
	Socket accept(std::string* clientIP = nullptr);
	int send(const char* buf, int len) override;
	int receive(char* buf, int len) override;

private:
	std::shared_ptr<SocketImpl> pImpl;
};
