#include "pch.h"

#include "FileServer.h"
#include "HttpRequest.h"
#include "StreamWriter.h"
#include "StringUtils.h"
#include "FileUploadHandler.h"
#include <sstream>
#include <filesystem>
#include <fstream>

net::FileServer::FileServer(const std::filesystem::path& rootFolder)
{
    _rootFolder = rootFolder;
}

net::FileServer::~FileServer()
{
    // Empty destructor prevents error about deletion of incomplete type. 
}

void net::FileServer::handleRequest(const HttpRequest& request, bool* keepAlive)
{
    _keepAlive = keepAlive;
    if (request.getHeader("connection") == "keep-alive")
        *_keepAlive = true;

    _streamWriter = std::make_unique<StreamWriter>(request.stream());

    if (request.hasFormData())
    {
        auto formData = request.readFormData();
        for (const auto& [id, file] : formData.files)
        {
            auto path = formData.values["path"];
            std::string dest = formData.values["path"] + file.fileName;
            std::filesystem::rename(file.tempName, std::filesystem::u8path(dest));
            serveString("302 Found", "", { "Location: " + formData.values["path"] });
            return;
        }
    }

    // Remove forward slash from HTTP request path.
    std::string resourceName = request.uri();
    if (resourceName.length() > 0 && resourceName[0] == '/')
        resourceName = resourceName.substr(1);

    auto path = _rootFolder / std::filesystem::u8path(resourceName);
    if (std::filesystem::is_directory(path))
    {
        //if (request.getHeader("authorization") == "")
        //    return serve401Unauthorized();
        serveDirectory(path);
    }
    else
    {
        serveFile(path);
    }
}

std::string net::FileServer::mimeType(const std::filesystem::path& path)
{
    auto ext = StringUtils::toLower(path.extension().u8string());
    if (ext == ".htm")    return "text/html";
    if (ext == ".html")   return "text/html";
    if (ext == ".php")    return "text/html";
    if (ext == ".css")    return "text/css";
    if (ext == ".txt")    return "text/plain";
    if (ext == ".pdf")    return "application/pdf";
    if (ext == ".ts")     return "application/octet-stream";
    if (ext == ".js")     return "application/javascript";
    if (ext == ".js.map") return "application/octet-stream";
    if (ext == ".json")   return "application/json";
    if (ext == ".xml")    return "application/xml";
    if (ext == ".swf")    return "application/x-shockwave-flash";
    if (ext == ".flv")    return "video/x-flv";
    if (ext == ".png")    return "image/png";
    if (ext == ".jpe")    return "image/jpeg";
    if (ext == ".jpeg")   return "image/jpeg";
    if (ext == ".jpg")    return "image/jpeg";
    if (ext == ".gif")    return "image/gif";
    if (ext == ".bmp")    return "image/bmp";
    if (ext == ".ico")    return "image/vnd.microsoft.icon";
    if (ext == ".tiff")   return "image/tiff";
    if (ext == ".tif")    return "image/tiff";
    if (ext == ".svg")    return "image/svg+xml";
    if (ext == ".svgz")   return "image/svg+xml";
    if (ext == ".mp4")    return "video/mp4";
    if (ext == ".mkv")    return "video/x-matroska";
    return "application/text";
}

void net::FileServer::serveString(std::string_view status, std::string_view body)
{
    return serveString(status, body, {});
}

void net::FileServer::serveString(std::string_view status, std::string_view body, const std::list<std::string>& extraHeaders)
{
    std::stringstream ss;
    ss << "HTTP/1.1 " << status << "\r\n";
    ss << "Connection: " << connectionStr() << "\r\n";
    ss << "Content-Type: text/html; charset=utf-8\r\n";
    ss << "Content-Length: " << body.length() << "\r\n";
    ss << "Server: " << "DemoServer\r\n";
    for (const auto& header : extraHeaders)
        ss << header << "\r\n";
    ss << "\r\n";
    std::string headers = ss.str();
    _streamWriter->write(headers);
    _streamWriter->write(body);
}

void net::FileServer::serve404NotFound()
{
    serveString("404 Not Found", "<h1>404 File not found.</h1>\n");
}

void net::FileServer::serve401Unauthorized()
{
    serveString("401 Unauthorized", "", {"WWW-Authenticate: Basic realm=\"File Server\", charset=\"UTF-8\""});
}

void net::FileServer::serveFile(const std::filesystem::path& path)
{
    std::string mediaType = mimeType(path);
    std::ifstream fs;
    fs.open(path, std::ios_base::binary | std::ios::ate);
    if (fs)
    {
        long sz = (long)fs.tellg();
        fs.seekg(0);

        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n";
        ss << "Connection: " << connectionStr() << "\r\n";
        ss << "Content-Type: " << mediaType << "; charset=utf-8\r\n";
        ss << "Content-Length: " << sz << "\r\n";
        ss << "Server: " << "DemoServer\r\n";
        ss << "\r\n";
        std::string headers = ss.str();
        _streamWriter->write(headers);

        char buff[4096];
        while (!fs.eof())
        {
            fs.read(buff, sizeof(buff));
            std::streamsize bytesRead = fs.gcount();
            _streamWriter->write(buff, (int)bytesRead);
        }
        fs.close();
    }
    else
    {
        serve404NotFound();
    }
}

void net::FileServer::serveDirectory(const std::filesystem::path& path)
{
    std::stringstream body;
    body << "<!DOCTYPE html><html><head><title>Directory listing</title></head>\n";
    body << "<body>\n";
   
    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        std::string displayName;
        std::string href;
        auto name = entry.path().filename().u8string();
        if (entry.is_directory())
        {
            displayName = std::string("[") + name + "]";
            href = name + "/";
        }
        else
        {
            displayName = name;
            href = name;
        }
        body << "<a href=\"" << href << "\">" << displayName << "</a><br />\n";
    }
    body << "<form action=\"/upload.asp\" method=\"post\" enctype=\"multipart/form-data\">";
    body << "  <input type=\"hidden\" name=\"path\" value=\"" << path.u8string() << "\">";
    body << "  <br>File: <input type=\"file\" name=\"fname\"><br>";
    body << "  <input type=\"submit\" value=\"Upload\">";
    body << "</form>";
    body << "</body></html>\n";
    serveString("200 OK", body.str());
}

std::string net::FileServer::connectionStr() const
{
    return *_keepAlive ? "keep-alive" : "close";
}