#include "pch.h"

#include "ContentReader.h"
#include "Stream.h"
#include <algorithm>
#include <limits>

net::ContentChunk::operator bool() const
{
    return size > 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

net::ContentReader::ContentReader(InputStream* stream, int contentLength, bool contentLengthValid)
{
    auto maxContentLength = std::numeric_limits<decltype(_remaining)>::max();
    _stream = stream;
    _remaining = contentLengthValid ? contentLength : maxContentLength;
    _buffSize = (int)sizeof(_buff);
}

net::ContentChunk net::ContentReader::readChunk()
{
    if (_remaining > 0)
    {
        int n = _stream->receive(_buff, std::min(_buffSize, _remaining));
        if (n > 0)
        {
            _remaining -= n;
            return { _buff, n };
        }
        else
        {
            // Client connection has closed, indicating the end of the content. 
            _remaining = 0;
        }
    }
    // Return an empty chunk to indicate that there are no more chunks to follow.  
    return { _buff, 0 }; 
}
