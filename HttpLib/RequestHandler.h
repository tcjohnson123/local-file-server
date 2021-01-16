#pragma once

namespace net
{
	class HttpRequest;

	class RequestHandler
	{
	public:
		virtual void handleRequest(const HttpRequest& request, bool* keepAlive) = 0;
	};
}
