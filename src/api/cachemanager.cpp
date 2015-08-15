#include "cachemanager.h"

std::map<std::string, Resource> CacheManager::_resources;
std::mutex CacheManager::_putLock;

Resource CacheManager::GetItem(const std::string& path)
{
    auto item = CacheManager::_resources.find(path);
    if(item != CacheManager::_resources.end())
        return item->second;

    return {};
}

void CacheManager::PutItem(const std::pair<std::string, Resource> &&item)
{
    std::lock_guard<std::mutex> lock(_putLock);
    CacheManager::_resources.insert(item);
}
