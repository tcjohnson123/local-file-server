#pragma once

#include <memory>

namespace net
{
	class RequestHandler;

	class RequestHandlerFactory
	{
	public:
		virtual std::unique_ptr<RequestHandler> createRequestHandler() = 0;
	};
}
