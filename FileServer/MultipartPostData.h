#pragma once

#include <string>
#include <map>

namespace net
{
    class UploadedFile
    {
    public:
        std::string tempName;
        std::string fileName;
    };

    class MultipartPostData
    {
    public:
        MultipartPostData(const std::string& boundary);
        virtual ~MultipartPostData();

        void processChar(char ch);

    private:
        void processChunk(char* chunk, int size);
        bool isBoundary(char* chunk, int size);

    public:
        std::map<std::string, std::string> data;
        std::map<std::string, UploadedFile> files;

    private:
        char _chunk[256];
        int _chunkSize;
        int _index;
        int _state;
        std::string _boundary;
        bool _isFile;
        std::string _name;
        FILE* _fp;
        int _numWrites;
    };
}