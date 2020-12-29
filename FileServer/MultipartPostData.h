#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <map>

namespace net
{
    class UploadedFile
    {
    public:
        std::filesystem::path tempName;
        std::filesystem::path fileName;
    };

    class MultipartPostData
    {
    public:
        MultipartPostData(std::string_view boundary);
        MultipartPostData(const MultipartPostData& other) = delete;
        MultipartPostData& operator=(const MultipartPostData& rhs) = delete;
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
        std::ofstream _fs;
        int _numWrites;
    };
}