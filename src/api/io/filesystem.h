#ifndef FILE_H
#define FILE_H

#include <vector>
#include <string>
#include <stdexcept>

namespace IO {

namespace FileSystem {
    struct fs_error : public std::runtime_error {
        fs_error() = default;
        fs_error(const std::string& msg) : std::runtime_error(msg) {}

        ~fs_error() = default;
      };

  std::vector<char> ReadFile(const std::string& path);
  std::string GetCurrentDirectory();
  bool FileExists(const std::string& path);
}
}

#endif // FILE_H
