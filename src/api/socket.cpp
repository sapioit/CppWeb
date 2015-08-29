//
// Created by Vladimir on 8/2/2015.
//

#include "socket.h"
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <system_error>
#include "log.h"
#include <assert.h>

using namespace IO;

Socket::Socket(std::uint16_t port) : _port(port) {
  _fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (_fd == -1)
    throw std::runtime_error("Could not create socket");
  int opt_res;
  opt_res = setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &_opt, sizeof(_opt));
  if (opt_res < -1)
    throw std::runtime_error("Setsockopt error");
  _address.sin_family = AF_INET;
  _address.sin_addr.s_addr = INADDR_ANY;
  _address.sin_port = htons(port);
}

Socket::Socket(int fd, bool connection) : _fd(fd), _connection(connection) {
}

Socket::Socket(const Socket& other) {
  _fd = dup(other.get_fd());  //, other.get_fd());
  if (!other.is_blocking())
    MakeNonBlocking();
}

Socket& Socket::operator=(const Socket& other) {
  _fd = dup(other.get_fd());  //, other.get_fd());
  if (!other.is_blocking())
    MakeNonBlocking();

  return *this;
}

void Socket::Bind() {
  int bind_result = ::bind(_fd, reinterpret_cast<struct sockaddr*>(&_address),
                           sizeof(_address));
  if (bind_result < 0) {
    if (errno == EADDRINUSE)
      throw std::runtime_error("Port " + std::to_string(_port) +
                               " is already in use");
    throw std::runtime_error("Bind failed, errno = " + std::to_string(errno));
  }
}

void Socket::Listen(int pending_max) {
  int listen_result = ::listen(_fd, pending_max);
  if (listen_result < 0)
    throw std::runtime_error("Listen failed");
}

void Socket::MakeNonBlocking() {
  int flags = fcntl(_fd, F_GETFL, 0);
  if (flags == -1)
    throw std::runtime_error("Could not get file descriptor flags");

  flags |= (_blocking ? (~O_NONBLOCK) : O_NONBLOCK);
  int result = fcntl(_fd, F_SETFL, flags);
  if (result == -1)
    throw std::runtime_error(
        "Could not set the non-blocking flag for the file descriptor");
}

std::shared_ptr<Socket> Socket::Accept() {
  struct sockaddr in_addr;
  socklen_t in_len;
  in_len = sizeof(in_addr);

  int accept_result = ::accept(_fd, &in_addr, &in_len);
  bool throw_exception = false;
  if (accept_result == -1) {
    if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
      if (_blocking) {
        throw_exception = true;
      } else
        return std::make_shared<Socket>(-1, true);
    }
  }
  if (throw_exception)
    throw std::runtime_error("Accept error, errno = " + std::to_string(errno));

  return std::make_shared<Socket>(accept_result, true);
}

ssize_t Socket::Write(const char* data, size_t size) {
  auto bytesWritten = ::write(_fd, static_cast<const void*>(data), size);
  if (bytesWritten == -1) {
    //        if (!(((errno == EAGAIN) || (errno == EWOULDBLOCK)) && _blocking))
    //        {
    //            throw std::runtime_error("Error when writing to socket, errno
    //            = " + std::to_string(errno));
    //        }
    return 0;
  } else
    return bytesWritten;
  return 0;
}

ssize_t Socket::Write(const std::vector<char>& vector) {
  return Write(vector.data(), vector.size());
}

int Socket::get_fd() const {
  return _fd;
}

void Socket::Close() {
  if (_fd != -1)
    ::close(_fd);
}

Socket::~Socket() {
  Close();
}

std::shared_ptr<Socket> Socket::start_socket(int port, int maxConnections) {
  try {
    auto socket = std::make_shared<IO::Socket>(port);
    (*socket).Bind();
    (*socket).MakeNonBlocking();
    (*socket).Listen(maxConnections);
    return socket;
  } catch (std::exception& ex) {
    throw;
  }
}

bool Socket::is_blocking() const {
  return _blocking;
}

ssize_t Socket::Write(const std::string& string) {
  try {
    return Write(string.data(), string.length());
  } catch (std::runtime_error& ex) {
    Log::e(ex.what());
    throw;
  }
}

bool Socket::WasShutDown() {
  char a;
  auto bytesRead = ::recv(_fd, &a, 1, MSG_PEEK);
  return bytesRead == 0;
}

bool Socket::operator<(const Socket& other) {
  return _reads < other._reads;
}
std::uint64_t Socket::getReads() const {
  return _reads;
}
bool Socket::getConnection() const {
  return _connection;
}

template <class T>
T Socket::Read(std::size_t size) {
  T result;

  try {
    if (size == 0) {
      auto bytesAvailable = [&]() -> int {
        int available = -1, errorCode;
        errorCode = ioctl(_fd, FIONREAD, &available);
        if (errorCode < 0)
          throw std::runtime_error(
              "Could not determine bytes available on socket with fd = " +
              std::to_string(_fd) + ". ioctl failed, errno = " +
              std::to_string(errno));
        return available;
      };

      ssize_t available = 0;
      try {
        available = static_cast<std::size_t>(bytesAvailable());
      } catch (std::runtime_error& ex) {
        throw;
      }
      result.resize(available);
      ssize_t readBytes = ::read(_fd, &result.front(), available);
      if (available != readBytes)
        throw std::runtime_error(
            "Socket read error on fd = " + std::to_string(_fd) +
            "."
            "Expected to read " +
            std::to_string(available) + " bytes, but could only read " +
            std::to_string(readBytes) + " bytes");
      ++_reads;
      return result;
    } else {
      result.resize(size);
      auto readBytes = ::read(_fd, &result.front(), size);
      if (readBytes == -1) {
        if (!(((errno == EAGAIN) || (errno == EWOULDBLOCK)) && _blocking))
          throw std::runtime_error("Error when reading from socket, errno = " +
                                   std::to_string(errno));
      } else if (static_cast<std::size_t>(readBytes) != size)
        result.resize(readBytes);
      ++_reads;
      return result;
    }
  } catch (std::system_error& ex) {
    throw std::runtime_error(
        "Caught system error while reading from socket. Text: " +
        std::string(ex.what()));
  }
}

template std::vector<char> Socket::Read<std::vector<char>>(std::size_t = 0);

template std::string Socket::Read<std::string>(std::size_t = 0);

std::string Socket::ReadUntil(const std::string& until, bool peek) {
  std::string result;
  constexpr std::size_t buffSize = 20;
  std::size_t sum = 0;
  do {
    result.resize(sum + buffSize);
    auto bytesRead = ::recv(_fd, &result.front(), buffSize + sum, MSG_PEEK);
    if (bytesRead == -1) {
      if (!(((errno == EAGAIN) || (errno == EWOULDBLOCK)) && _blocking))
        throw std::runtime_error("Error when reading from socket, errno = " +
                                 std::to_string(errno));
      else
        return "";
    }
    auto position = result.find(until.c_str(), 0, until.size());
    if (position != std::string::npos) {
      position += until.size();
      try {
        if (peek)
          return result.substr(0, position);
        return Read<std::string>(position);  // result.substr(0, position);
      } catch (std::runtime_error& ex) {
        throw;
      }
    } else
      sum += bytesRead;
  } while (true);
}
