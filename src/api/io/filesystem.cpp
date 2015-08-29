#include <io/filesystem.h>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

std::vector<char> IO::FileSystem::ReadFile(const std::string& path) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream.is_open())
    throw fs_error("File could not be opened");
  std::vector<char> fileContents((std::istreambuf_iterator<char>(stream)),
                                 std::istreambuf_iterator<char>());
  return fileContents;
}

std::string IO::FileSystem::GetCurrentDirectory() {
  char* cwd = NULL;
  cwd = getcwd(0, 0);
  std::string cur_dir(cwd);
  free(cwd);

  return cur_dir;
}


bool IO::FileSystem::FileExists(const std::string &path)
{
    struct stat st;
    auto result = ::stat(path.c_str(), &st);
    if(result == -1)
        return false;
    return true;
}
