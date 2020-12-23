#include "pch.h"
#include "StreamWriter.h"
#include "Stream.h"

net::StreamWriter::StreamWriter(OutputStream* stream)
{
    _stream = stream;
}

bool net::StreamWriter::write(const char* buff, int length)
{
    int n, total = 0;
    if (length == 0)
        return true;

    do
    {
        n = _stream->send(buff, length - total);
        buff += n;
        total += n;
    } while (n > 0 && length > total);
    return total == length;
}

bool net::StreamWriter::write(std::string_view str)
{
    return write(str.data(), (int) str.length());
}
