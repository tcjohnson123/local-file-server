#pragma once

#include "Stream.h"
#include <memory>
#include <string>

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
		bool valid() const;
		bool bind(const EndPoint& endPoint);
		bool listen();
		bool close();
		Socket accept(EndPoint* clientIP = nullptr);
		int send(const char* buf, int len) override;
		int sendTo(const char* buf, int len, const EndPoint& endPoint);
		int receive(char* buf, int len) override;

	private:
		std::shared_ptr<SocketImpl> pImpl;
	};
}
