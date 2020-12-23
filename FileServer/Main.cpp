#include "pch.h"
#include "Socket.h"
#include "FileRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "WebServer.h"
#include <iostream>

namespace net
{
    class FileRequestHandlerFactory : public RequestHandlerFactory
    {
    public:
        std::unique_ptr<RequestHandler> createRequestHandler() override
        {
            return std::make_unique<FileRequestHandler>();
        }
    };
}

int main(void)
{
    Socket::startUp();
    net::FileRequestHandlerFactory factory;
    net::WebServer webServer(factory);
    webServer.start();
    Socket::shutDown();
    return 0;
}
