#include "pch.h"

#include "ContentReader.h"
#include "Stream.h"
#include <algorithm>

net::ContentChunk::operator bool() const
{
    return size > 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

net::ContentReader::ContentReader(InputStream* stream, int contentLength, bool contentLengthValid)
{
	_stream = stream;
	_contentLength = contentLength;
	_contentLengthValid = contentLengthValid;
    _remaining = _contentLength;
    _buffSize = (int)sizeof(_buff);
    _finished = false;
}

net::ContentChunk net::ContentReader::readChunk()
{
    if (_contentLengthValid)
    {
        if (!_finished && _remaining > 0)
        {
            int n = _stream->receive(_buff, std::min(_buffSize, _remaining));
            if (n > 0)
            {
                _remaining -= n;
                return { _buff, n };
            }
        }
    }
    else
    {
        if (!_finished)
        {
            int n = _stream->receive(_buff, _buffSize);
            if (n > 0)
            {
                _remaining -= n;
                return { _buff, n };
            }
        }
    }
    _finished = true;
    return { _buff, 0 };
}
