#pragma once

#include "Stream.h"
#include <memory>
#include <string>
#include <vector>

namespace net
{
	class EndPointImpl;

	class EndPoint
	{
	public:
		friend class Socket;

	public:
		EndPoint();
		EndPoint(const char* ip, int port);
		EndPoint(const EndPoint& other);
		virtual ~EndPoint();

		std::string ipAddress() const;
		int port() const;

	private:
		std::unique_ptr<EndPointImpl> pImpl;
	};

	class SocketImpl;

	enum class Protocol
	{
		None,
		TCP,
		UDP
	};

	class Socket : public net::Stream
	{
	public:
		Socket(Protocol proto);

		static bool startUp();
		static void shutDown();
		static std::vector<std::string> getHostByName(const char* hostName);
		bool valid() const;
		bool bind(const EndPoint& endPoint);
		bool listen();
		bool close();
		Socket accept(EndPoint* clientIP = nullptr);
		int send(const char* buf, int len) override;
		int receive(char* buf, int len) override;
		int sendTo(const char* buf, int len, const EndPoint& endPoint);
		int receiveFrom(char* buf, int len, EndPoint* sourceAddress);
		EndPoint endPoint() const;
		bool connect(const EndPoint& endPoint);

	private:
		std::shared_ptr<SocketImpl> pImpl;
	};
}
