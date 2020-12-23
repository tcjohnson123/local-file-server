#pragma once
namespace net
{
	class InputStream
	{
	public:
		virtual int receive(char* buf, int len) = 0;
	};

	class OutputStream
	{
	public:
		virtual int send(const char* buf, int len) = 0;
	};

	class Stream : public InputStream, public OutputStream
	{
	};
}
