#include "pch.h"
#include "StreamReader.h"
#include "Stream.h"

net::StreamReader::StreamReader(InputStream* stream)
{
    _stream = stream;
}

bool net::StreamReader::readLine(std::string& str)
{
    int n;
    int total = 0;
    char buf[1];

    str.clear();
    while (1)
    {
        n = _stream->receive(&buf[0], sizeof(buf));
        total += n;
        if (n == 1)
        {
            char ch = buf[0];
            if (ch == 10)
                return true;
            else if ((ch >= ' ') || (ch == '\t'))
                str += buf[0];
        }
        else
        {
            return total > 0;
        }
    }
    return true;
}

