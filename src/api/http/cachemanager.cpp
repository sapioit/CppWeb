#include <http/cachemanager.h>
#include <io/filesystem.h>
#include <misc/storage.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

std::map<std::string, Resource> CacheManager::_resources;
std::mutex CacheManager::_putLock;

Resource CacheManager::GetItem(const std::string& path) {
  auto item = CacheManager::_resources.find(path);
  if (item != CacheManager::_resources.end()) return item->second;

  return {};
}

void CacheManager::PutItem(const std::pair<std::string, Resource>&& item) {
  std::lock_guard<std::mutex> lock(_putLock);
  CacheManager::_resources.insert(item);
}

void CacheManager::ReplaceItem(const std::string& path, const Resource& res) {
  _resources[path] = res;
}
Resource CacheManager::GetResource(const std::string& path) {
  std::string fpath(Storage::settings().root_path + path);

  auto item = CacheManager::GetItem(fpath);

  if (item) {
    struct stat st;
    auto st_res = stat(fpath.c_str(), &st);
    if (st_res != 0) {
      // TODO remove item from cache
      throw 404;
    } else {
      if (st.st_mtime > item.stat().st_mtime) {
        try {
          Resource res(fpath);
          CacheManager::ReplaceItem(fpath, res);
          return res;
        } catch (IO::fs_error& ex) {
          throw 404;
        } catch (std::system_error& ex) {
          throw 500;
        }
      } else
        return item;
    }
  } else {
    try {
      Resource res(fpath);
      CacheManager::PutItem(std::make_pair(fpath, res));
      return res;
    } catch (IO::fs_error& ex) {
      throw 404;
    } catch (std::system_error& ex) {
      throw 500;
    }
  }
}
