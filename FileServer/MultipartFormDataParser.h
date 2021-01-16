#pragma once

#include "FormDataParser.h"
#include "UploadedFile.h"
#include <string>
#include <fstream>

namespace net
{
    class FormDataHandler;
    class UploadHandler;

    class MultipartFormDataParser : public FormDataParser
    {
    public:
        MultipartFormDataParser(FormDataHandler* handler, std::string_view boundary);
        MultipartFormDataParser(const MultipartFormDataParser& other) = delete;
        MultipartFormDataParser& operator=(const MultipartFormDataParser& rhs) = delete;
        virtual ~MultipartFormDataParser();

        void processChars(char* data, int count) override;
        void endOfStream() override;

    private:
        void processChunk(char* chunk, int size);
        bool isBoundary(char* chunk, int size);
        void closeStream();
        void processChar(char ch);

    private:
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