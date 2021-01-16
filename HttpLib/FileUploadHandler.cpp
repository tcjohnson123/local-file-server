#include "pch.h"

#include "FileUploadHandler.h"
#include "StringUtils.h"

net::FileUploadHandler::FileUploadHandler(std::string_view id, std::string_view fileName)
{
    _id = id;
    _fileName = fileName;

    std::ostringstream tempName;
    tempName << "upload_" << StringUtils::randomString(8) << "_" << fileName;
    _tempName = std::filesystem::temp_directory_path() / std::filesystem::u8path(tempName.str());
    _fs.open(_tempName, std::ios_base::binary);
}

void net::FileUploadHandler::write(char* data, int size)
{
    _fs.write(data, static_cast<int>(size));
}

void net::FileUploadHandler::endOfStream()
{
    _fs.close();
}

std::filesystem::path net::FileUploadHandler::tempName() const
{
    return _tempName;
}
