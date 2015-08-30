#include <io/scheduled_write.h>
#include <utility>

IO::scheduled_write::~scheduled_write()
{
    if (_moved == false)
        for (auto index = 0u; index < _packets.size(); ++index) {
            delete _packets.front().first;
            _packets.pop();
        }
}

void IO::scheduled_write::pop()
{
    delete _packets.front().first;
    _packets.pop();
    Log::i("Have popped scheduled item");
}

IO::scheduled_write::scheduled_write(IO::scheduled_write&& other)
{
    operator=(std::move(other));
}

IO::scheduled_write& IO::scheduled_write::operator=(IO::scheduled_write&& other)
{
    _sock = other._sock;
    _packets.swap(other._packets);
    return *this;
}
