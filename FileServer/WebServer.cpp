#include "pch.h"

#include "WebServer.h"
#include "RequestHandlerFactory.h"
#include "Socket.h"
#include "StreamWriter.h"
#include "HttpRequest.h"
#include "RequestHandler.h"
#include <iostream>
#include <thread>

static void clientThreadFn(Socket socket, const std::string& clientIP, net::RequestHandlerFactory& factory)
{
    net::HttpRequest request(&socket, clientIP.c_str());
    auto requestHandler = factory.createRequestHandler();
    requestHandler->handleRequest(request);

    // Force the client to close the connection
    char dummy[256];
    int nb = socket.receive(dummy, sizeof(dummy));

    socket.close();
}

net::WebServer::WebServer(RequestHandlerFactory& factory) : _factory(factory)
{
}

bool net::WebServer::start()
{
    Socket socket(Protocol::TCP);
    socket.bind("0.0.0.0", 8080);
    bool rc = socket.listen();

    while (true)
    {
        std::string clientIP;
        auto client = socket.accept(&clientIP);
        if (!client.valid())
        {
            perror("accept() error");
        }

        std::thread clientThread(
            [=]() {clientThreadFn(client, clientIP, _factory);}
        );
        clientThread.detach();
    }

    std::cout << "Program complete." << std::endl;
    socket.close();
}

