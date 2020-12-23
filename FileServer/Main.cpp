#include "pch.h"
#include "Socket.h"
#include "FileServer.h"
#include "RequestHandler.h"
#include "WebServer.h"
#include <iostream>

namespace net
{
    class FileRequestHandler : public RequestHandler
    {
    public:
        void handleRequest(const HttpRequest& request) override
        {
            FileServer fileServer;
            fileServer.handleRequest(request);
        }
    };
}

int main(void)
{
    Socket::startUp();
    net::FileRequestHandler handler;
    net::WebServer webServer(handler);
    webServer.start();
    Socket::shutDown();
    return 0;
}
