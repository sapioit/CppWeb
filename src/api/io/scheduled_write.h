#ifndef SCHEDULED_WRITE_H
#define SCHEDULED_WRITE_H

#include <vector>
#include <io/datapacket.h>
#include <io/socket.h>
#include <misc/dfa.h>

struct scheduled_write {

  enum class WriteState { Idle, InProgress, Done };
  enum class WriteResult { Finalized, NotFinalized };
  std::shared_ptr<IO::Socket> sock;
  std::vector<char> data;

  scheduled_write(std::shared_ptr<IO::Socket> s, const std::string& d)
      : sock(s), data(d.begin(), d.end()) {}

  ~scheduled_write() = default;
  scheduled_write(scheduled_write&&) = default;
  scheduled_write(const scheduled_write&) = default;
  scheduled_write& operator=(const scheduled_write&) = default;

  class PacketWriteProgress;
  std::vector<std::pair<IO::DataPacket*, PacketWriteProgress> > packets;

  using S = WriteState;
  using T = WriteResult;
  class PacketWriteProgress : DFA<S, T> {
    PacketWriteProgress() : DFA<S, T>(S::Idle, S::Done) {
      DFA<S, T>::add({ S::Idle, T::Finalized }, S::Done);
      DFA<S, T>::add({ S::Idle, T::NotFinalized }, S::InProgress);
      DFA<S, T>::add({ S::InProgress, T::Finalized }, S::InProgress);
      DFA<S, T>::add({ S::InProgress, T::Finalized }, S::Done);
    }
  };
};
#endif // SCHEDULED_WRITE_H
