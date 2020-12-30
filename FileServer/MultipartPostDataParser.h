#pragma once

#include "PostDataParser.h"
#include "UploadedFile.h"
#include <string>
#include <fstream>

namespace net
{
    class PostDataHandler;
    class UploadHandler;

    class MultipartPostDataParser : public PostDataParser
    {
    public:
        MultipartPostDataParser(PostDataHandler* handler, std::string_view boundary);
        MultipartPostDataParser(const MultipartPostDataParser& other) = delete;
        MultipartPostDataParser& operator=(const MultipartPostDataParser& rhs) = delete;
        virtual ~MultipartPostDataParser();

        void processChar(char ch) override;
        void endOfStream() override;

    private:
        void processChunk(char* chunk, size_t size);
        bool isBoundary(char* chunk, size_t size);

    private:
        UploadedFile _uploadedFile;
        char _chunk[256];
        int _chunkSize;
        int _index;
        int _state;
        std::string _boundary;
        bool _isFile;
        std::string _name;
        std::unique_ptr<UploadHandler> _fs;
        int _numWrites;
    };
}