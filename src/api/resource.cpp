#include "resource.h"
#include "filesystem.h"

const uint64_t &Resource::hits() const
{
    return _hits;
}

const std::vector<char> &Resource::content() const
{
    return _content;
}

const std::string &Resource::path() const
{
    return _path;
}


struct stat Resource::stat() const
{
    return _stat;
}
Resource::Resource(const std::string & path, const std::vector<char> &content, const struct stat& stat) : _path(path), _content(content), _stat(stat)
{

}

Resource::Resource(const std::string & path) :_path(path)
{
    try {
        _content = std::move(IO::FileSystem::ReadFile(path));
        auto stat_res = ::stat(_path.c_str(), &_stat);
        if(stat_res != 0)
            throw IO::fs_error("Could not stat " + path);
    }
    catch(IO::fs_error &ex) {
        throw;
    }
}

bool Resource::operator <(const Resource &other)
{
    return _hits < other.hits();
}

Resource::operator bool()
{
    return (_content.size() != 0);
}

