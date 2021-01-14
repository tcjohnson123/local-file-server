#include "pch.h"
#include "MultipartPostDataParser.h"
#include "Property.h"
#include "StringUtils.h"
#include "PostDataHandler.h"
#include <string>
#include <list>
#include <string.h>

net::MultipartPostDataParser::MultipartPostDataParser(PostDataHandler* handler, std::string_view boundary)
    : PostDataParser(handler)
{
    _handler = handler;
    _boundary = boundary;
    _index = 0;
    _state = 0;
    _isFile = false;
    _chunk[0] = 0;
    _chunkSize = sizeof(_chunk);
    _numWrites = 0;
}

net::MultipartPostDataParser::~MultipartPostDataParser()
{
    endOfStream();
}

void net::MultipartPostDataParser::processChar(char ch)
{
    if (ch == 13 || _index >= _chunkSize)
    {
        processChunk(_chunk, _index);
        _index = 0;
    }
    _chunk[_index++] = ch;
}

void net::MultipartPostDataParser::closeStream()
{
    if (_fs && _fs->is_open())
        _fs->close();
    _fs.reset();
}

void net::MultipartPostDataParser::endOfStream()
{
    closeStream();
}

bool net::MultipartPostDataParser::isBoundary(char* chunk, size_t size)
{
    auto boundaryLength = _boundary.length();
    if (size == boundaryLength + 2)
    {
        if (strncmp(&chunk[2], _boundary.c_str(), size - 2) == 0)
            return true;
    }
    else if (size == boundaryLength + 4)
    {
        if (strncmp(&chunk[4], _boundary.c_str(), size - 4) == 0)
            return true;
    }
    else if (size == boundaryLength + 6)
    {
        if (strncmp(&chunk[4], _boundary.c_str(), size - 6) == 0)
            return true;
    }
    return false;
}

void net::MultipartPostDataParser::processChunk(char* chunk, size_t size)
{
    if (_state == 0) // Waiting for boundary
    {
        if (isBoundary(chunk, size))
        {
            _isFile = false;
            _state = 1;
        }
    }
    else if (_state == 1) // Expecting header line
    {
        if (size == 2)
        {
            _state = 2;
        }
        else
        {
            std::string header(chunk + 2, size - 2);
            std::list<Property> props;
            StringUtils::parseNameValuePairs(props, header.c_str(), ';');
            for (const auto& prop : props)
            {
                if (prop.name == "name")
                {
                    _name = prop.value;
                }
                else if (prop.name == "filename")
                {
                    _isFile = true;
                    _numWrites = 0;
                    _fs = _handler->createStreamForUpload(_name, prop.value);
                }
            }
        }
    }
    else if (_state == 2) // Expecting data
    {
        if (isBoundary(chunk, size))
        {
            _isFile = false;
            closeStream();
            _state = 1;
        }
        else
        {
            if (_isFile)
            {
                if (_fs && *_fs)
                {
                    if (_numWrites++ == 0)
                        _fs->write(chunk + 2, size - 2);
                    else
                        _fs->write(chunk, size);
                }
            }
            else if (size > 2)
            {
                std::string value(chunk + 2, size - 2);
                _handler->addDataPair(_name, value);
            }
        }
    }
} 
