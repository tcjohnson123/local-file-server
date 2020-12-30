#include "pch.h"
#include "UploadFileWriter.h"

net::UploadFileWriter::UploadFileWriter(std::string_view fname)
{
	//_tempName = std::filesystem::temp_directory_path() / fname;
	//_fs.open(_tempName, std::ios_base::binary);
	_fs.open(fname, std::ios_base::binary);
}

bool net::UploadFileWriter::handleChunk(const char* data, size_t size)
{
	_fs.write(data, size);
	return true;
}

void net::UploadFileWriter::endOfStream()
{
	_fs.close();
}
