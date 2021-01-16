#pragma once

namespace net
{
	class RequestHandler;

	class WebServer
	{
	public:
		WebServer(RequestHandler& handler);
		bool start();

	private:
		RequestHandler& _handler;
	};
}
