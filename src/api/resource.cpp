#include "resource.h"


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

Resource::Resource(const std::string & path, const std::vector<char> & content) : _path(path), _content(content)
{

}

bool Resource::operator <(const Resource &other)
{
    return _hits < other.hits();
}

Resource::operator bool()
{
    return (_content.size() != 0);
}

