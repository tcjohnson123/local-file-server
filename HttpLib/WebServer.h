#pragma once

#include <string>

namespace net
{
	class RequestHandler;

	class WebServer
	{
	public:
		WebServer(RequestHandler& handler);
		bool start(const std::string& ip, int port);

	private:
		RequestHandler& _handler;
	};
}
