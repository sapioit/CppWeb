#ifndef SCHEDULED_WRITE_H
#define SCHEDULED_WRITE_H

#include <io/datapacket.h>
#include <io/socket.h>
#include <misc/dfa.h>
#include <misc/log.h>
#include <algorithm>
#include <queue>
#include <iostream>
#include <sstream>

namespace IO {
class scheduled_write {
public:
    enum class WriteState;
    enum class WriteResult;
    class PacketWriteProgress;

private:
    std::shared_ptr<IO::Socket> _sock;
    bool _moved = false;
    std::queue<std::pair<DataPacket*, PacketWriteProgress> > _packets;

public:
    enum class WriteState { Idle = 0x0,
        InProgress = 0x1,
        Done = 0x2 };
    enum class WriteResult { Finalized = 0x0,
        NotFinalized = 0x1 };

    /* initializer for variadic template*/
    template <typename... Args>
    scheduled_write(std::shared_ptr<IO::Socket> sock, Args... args)
        : _sock(sock)
    {
        push(std::forward<Args>(args)...);
        Log::i("queue size = " + std::to_string(_packets.size()));
//        Log::i("dumping scheduled item: ");
//        Log::i(dump());
    }

    template <typename T>
    void push(T packet)
    {
        _packets.push(std::make_pair(packet, PacketWriteProgress()));
    }

    template <typename T, typename... Targs>
    void push(T first, Targs... args)
    {
        push(first);
        push(args...);
    }

    scheduled_write() = default;
    scheduled_write(scheduled_write&&);
    scheduled_write(const scheduled_write&) = delete;
    scheduled_write& operator=(scheduled_write&& other);
    ~scheduled_write();

    std::string dump() {
        std::ostringstream dump;
        dump << std::endl << "packet: " << this << std::endl;
        dump << "sockfd = " << _sock.get()->get_fd() << std::endl;
        dump << std::to_string(packets_left())  <<  " packets left" << std::endl;
        dump << "front of the packet queue: " <<  std::to_string(current_packet().left())  <<  " bytes left" << std::endl;
        dump << "type : " << typeid(current_packet()).name() << std::endl;
        if(typeid(current_packet()).hash_code() == typeid(IO::MemoryPacket<std::string>).hash_code()) {
            dump << "Content: " << std::endl << dynamic_cast<IO::MemoryPacket<std::string>&>(current_packet()).rep << std::endl;
        }
        dump << "state: " << std::to_string((int)current_packet_state().currentState())  << std::endl;
        dump << "Finished dumping scheduled item"  << std::endl;
        return dump.str();
    }

    decltype(_sock) sock() const { return _sock; }
    DataPacket& current_packet() { return *_packets.front().first; }
    DFA<WriteState, WriteResult>& current_packet_state() { return _packets.front().second; }
    void pop();
    decltype(_packets.size()) packets_left() { return _packets.size(); }

    using S = WriteState;
    using T = WriteResult;
    class PacketWriteProgress : public DFA<S, T> {
    public:
        PacketWriteProgress()
            : DFA<S, T>(S::Idle, S::Done)
        {
            add({ S::Idle, T::Finalized }, S::Done);
            add({ S::Idle, T::NotFinalized }, S::InProgress);
            add({ S::InProgress, T::Finalized }, S::InProgress);
            add({ S::InProgress, T::Finalized }, S::Done);
        }
    };
};
}
#endif // SCHEDULED_WRITE_H
