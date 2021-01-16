#include "pch.h"
#include "MultipartFormDataParser.h"
#include "Property.h"
#include "StringUtils.h"
#include "FormDataHandler.h"
#include "UploadHandler.h"
#include <string>
#include <list>
#include <string.h>

net::MultipartFormDataParser::MultipartFormDataParser(FormDataHandler* handler, std::string_view boundary)
    : FormDataParser(handler)
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

net::MultipartFormDataParser::~MultipartFormDataParser()
{
    endOfStream();
}

void net::MultipartFormDataParser::processChars(char* data, int count)
{
    for (int i = 0; i < count; i++)
        processChar(data[i]);
}

void net::MultipartFormDataParser::processChar(char ch)
{
    if (ch == 13 || _index >= _chunkSize)
    {
        processChunk(_chunk, _index);
        _index = 0;
    }
    _chunk[_index++] = ch;
}

void net::MultipartFormDataParser::closeStream()
{
    if (_fs)
        _fs->endOfStream();
    _fs.reset();
}

void net::MultipartFormDataParser::endOfStream()
{
    closeStream();
}

bool net::MultipartFormDataParser::isBoundary(char* chunk, int size)
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

void net::MultipartFormDataParser::processChunk(char* chunk, int size)
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
            StringUtils::parseNameValuePairs(props, header, ';');
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
                    _fs = _handler->createUploadHandler(_name, prop.value);
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
                if (_fs)
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
