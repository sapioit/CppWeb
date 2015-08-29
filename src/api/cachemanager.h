#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <vector>
#include "resource.h"
#include <string>
#include <map>
#include <mutex>

class CacheManager {
  static std::map<std::string, Resource> _resources;
  static std::mutex _putLock;

 public:
  static Resource GetItem(const std::string&);
  static void PutItem(const std::pair<std::string, Resource>&&);
  static void ReplaceItem(const std::string&, const Resource&);
  static Resource GetResource(const std::string& path);
};

#endif  // CACHEMANAGER_H
