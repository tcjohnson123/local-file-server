#pragma once

#include "FormDataParser.h"
#include "UploadedFile.h"
#include <string>
#include <fstream>

namespace net
{
    class FormDataHandler;

    class MultipartFormDataParser : public FormDataParser
    {
    public:
        MultipartFormDataParser(FormDataHandler* handler, std::string_view boundary);
        MultipartFormDataParser(const MultipartFormDataParser& other) = delete;
        MultipartFormDataParser& operator=(const MultipartFormDataParser& rhs) = delete;
        virtual ~MultipartFormDataParser();

        void processChar(char ch) override;
        void endOfStream() override;

    private:
        void processChunk(char* chunk, size_t size);
        bool isBoundary(char* chunk, size_t size);
        void closeStream();

    private:
        char _chunk[256];
        int _chunkSize;
        int _index;
        int _state;
        std::string _boundary;
        bool _isFile;
        std::string _name;
        std::unique_ptr<std::ofstream> _fs;
        int _numWrites;
    };
}