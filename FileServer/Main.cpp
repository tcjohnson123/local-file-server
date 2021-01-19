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
        void handleRequest(const HttpRequest& request, bool* keepAlive) override
        {
            FileServer fileServer;
            fileServer.handleRequest(request, keepAlive);
        }
    };
}

void showUsage()
{
    std::cout << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << std::endl;
    std::cout << "   FileServer [-p <PORT>] [-a <IP_ADDRESS>]" << std::endl;
    std::cout << std::endl;
    std::cout << "Defaults:" << std::endl;
    std::cout << std::endl;
    std::cout << "   PORT = 8080" << std::endl;
    std::cout << "   IP_ADDRESS = 127.0.0.1" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    int port = 8080;
    std::string ip = "127.0.0.1";

    if (argc % 2 == 0)
    {
        showUsage();
        return 0;
    }

    for (int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            port = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            ip = argv[i + 1];
        }
        else
        {
            showUsage();
            return 0;
        }
    }

    net::Socket::startUp();
    net::FileRequestHandler handler;
    net::WebServer webServer(handler);
    webServer.start(ip, port);
    net::Socket::shutDown();
    return 0;
}
