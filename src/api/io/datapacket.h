#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <io/filesystem.h>
namespace IO {
class DataPacket {};

template <class T> class MemoryPacket : public DataPacket {
public:
  T rep;
  MemoryPacket(const T& p) : rep(p) {}
};

class File : DataPacket {
  int _fd;
  std::uint64_t _size;
  off_t _last_offset;
  std::string _path;
  inline IO::FileSystem::fs_error could_not_dup() {
    return { "Could not dup fd = " + std::to_string(_fd) };
  }

public:
  File(const std::string& path) : _path(path) {
    _fd = ::open(path.c_str(), O_RDONLY);
    if (_fd == -1)
      throw IO::FileSystem::fs_error("Could not open " + path);
    struct stat64 st;
    auto result = ::stat64(path.c_str(), &st);
    if (result == -1)
      throw IO::FileSystem::fs_error("Could not stat file " + path);
    _size = st.st_size;
  }
  File(const File& other) {
    _fd = ::dup(other._fd);
    if (_fd == -1)
      throw could_not_dup();
  }
  File(File&& other) {
    _fd = ::dup(other._fd);
    if (_fd == -1)
      throw could_not_dup();
    other._fd = -1;
  }
  File& operator=(const File& other) {
    _fd = ::dup(other._fd);
    if (_fd == -1)
      throw could_not_dup();
    return *this;
  }
  ~File() noexcept {
    if (_fd != -1)
      ::close(_fd);
  }

  int fd() const noexcept { return _fd; }

  void adjust_offset(off_t off) {
    auto result = ::lseek(_fd, off, SEEK_SET);
    if (result == -1)
      throw IO::FileSystem::fs_error(
          "Could not seek on fd = " + std::to_string(_fd) + " refering to " + _path + ". File size = " +
          std::to_string(_size) + " last offset = " +
          std::to_string(_last_offset) + ". Tried to set offset at " +
          std::to_string(off));
    _last_offset += off;
  }

  off_t offset() const {
    auto off = ::lseek(_fd, 0, SEEK_CUR);
        throw IO::FileSystem::fs_error("Could not lseek to get offset for fd = " + _fd);
    return off;
  }

  off_t last_offset() const noexcept { return _last_offset; }
  const std::uint64_t& size() const noexcept { return _size; }
};
}

#endif // DATAPACKET_H
