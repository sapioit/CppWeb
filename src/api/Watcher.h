//
// Created by Vladimir on 8/2/2015.
//

#ifndef SOCKET_OBSERVER_H
#define SOCKET_OBSERVER_H

#include "Socket.h"
#include <sys/epoll.h>
#include <memory>
#include <vector>

namespace IO {
    class Watcher {
    public:
        Watcher(std::shared_ptr<Socket>
                socket);

        void Stop();

        void Start(std::function<void(std::shared_ptr<Socket>)> callback);

        ~Watcher();

    private:
        static constexpr int _maxEvents = 30;
        std::vector<std::shared_ptr<Socket>> _to_observe;
        std::shared_ptr<Socket> _socket;
        int _efd;
        std::vector<epoll_event> _events;
        bool _stopRequested = false;


        std::vector<std::shared_ptr<Socket>> Watch();

        void AddSocket(std::shared_ptr<Socket> socket);
    };
};

#endif //SOCKET_OBSERVER_H
