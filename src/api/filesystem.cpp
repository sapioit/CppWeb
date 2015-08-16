#include "filesystem.h"
#include <fstream>
using namespace IO;

std::vector<char> FileSystem::ReadFile(const std::string &path)
{
    std::ifstream stream(path, std::ios::binary);
    if(!stream.is_open())
        throw fs_error("File could not be opened");
    std::vector<char> fileContents((std::istreambuf_iterator<char>(stream)),
                                   std::istreambuf_iterator<char>());
    return fileContents;
}
