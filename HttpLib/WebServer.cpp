#include "pch.h"

#include "WebServer.h"
#include "Socket.h"
#include "StreamWriter.h"
#include "HttpRequest.h"
#include "RequestHandler.h"
#include <iostream>
#include <thread>

static void clientThreadFn(net::Socket socket, const std::string& clientIP, net::RequestHandler& handler)
{
    net::HttpRequest request(&socket, clientIP);
    handler.handleRequest(request);

    // Force the client to close the connection
    char dummy[256];
    int nb = socket.receive(dummy, sizeof(dummy));

    socket.close();
}

net::WebServer::WebServer(RequestHandler& handler) : _handler(handler)
{
}

bool net::WebServer::start()
{
    Socket socket(Protocol::TCP);
    socket.bind(EndPoint("0.0.0.0", 8080));
    bool rc = socket.listen();

    while (true)
    {
        EndPoint clientEndPoint;
        auto client = socket.accept(&clientEndPoint);
        auto clientIP = clientEndPoint.ipAddress();
        if (!client.valid())
        {
            perror("accept() error");
        }

        std::thread clientThread(
            [=]() {clientThreadFn(client, clientIP, _handler);}
        );
        clientThread.detach();
    }

    std::cout << "Program complete." << std::endl;
    socket.close();
}

