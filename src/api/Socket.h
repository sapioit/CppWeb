//
// Created by Vladimir on 8/2/2015.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <memory>
#include <unistd.h>
#include <vector>

namespace IO {
class Socket {
public:
    int get_fd() const;
    bool is_blocking() const;
    Socket(int port, bool connection);
    Socket(const Socket &) = delete;
    Socket operator=(const Socket &) = delete;
    virtual ~Socket();
    static std::shared_ptr<Socket> start_socket(int port, int maxConnections);
    void Close();
    void Bind();
    void MakeNonBlocking();
    void Listen(int pending_max);
    std::shared_ptr<Socket> Accept();
    Socket(std::uint16_t fd);

    template<class T>
    T Read(std::size_t size = 0);

    std::string ReadUntil(const std::string &U, bool peek = false);
    void Write(const char *, std::size_t);
    void Write(const std::vector<char> &);
    void Write(const std::string &);

    bool operator<(const Socket&);
    std::uint64_t getReads() const;

private:
    std::uint16_t _port;
    std::uint64_t _reads = 0;
    int _opt = 1, _fd = -1;
    struct sockaddr_in _address;
    bool _blocking = false;

};
};

#endif //SOCKET_SOCKET_H
