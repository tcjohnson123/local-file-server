#pragma once

#include "UploadedFile.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <map>

namespace net
{
    class PostDataHandler;

    class MultipartPostDataParser
    {
    public:
        MultipartPostDataParser(PostDataHandler* handler, std::string_view boundary);
        MultipartPostDataParser(const MultipartPostDataParser& other) = delete;
        MultipartPostDataParser& operator=(const MultipartPostDataParser& rhs) = delete;
        virtual ~MultipartPostDataParser();

        void processChar(char ch);

    private:
        void processChunk(char* chunk, size_t size);
        bool isBoundary(char* chunk, size_t size);

    private:
        PostDataHandler* _handler;
        UploadedFile _uploadedFile;
        char _chunk[256];
        int _chunkSize;
        int _index;
        int _state;
        std::string _boundary;
        bool _isFile;
        std::string _name;
        std::ofstream _fs;
        int _numWrites;
    };
}