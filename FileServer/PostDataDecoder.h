#pragma once

namespace net
{
	class PostDataHandler;
	class InputStream;

	class PostDataDecoder
	{
	public:
		PostDataDecoder(PostDataHandler* handler, const std::string& contentTypeHeader, const std::string& contentLengthHeader);

		bool decode(InputStream* stream);

	private:
		void parseContentTypeHeader(const std::string& contentTypeHeader);
		void parseContentLengthHeader(const std::string& contentLengthHeader);

	private:
		PostDataHandler* _handler;
		std::string _boundary;
		int _contentLength;
		std::string _contentType;
		bool _contentLengthValid;
	};
}
