#pragma once

namespace net
{
	class RequestHandlerFactory;

	class WebServer
	{
	public:
		WebServer(RequestHandlerFactory& factory);
		bool start();

	private:
		RequestHandlerFactory& _factory;
	};
}
