#include "storage.h"
#include "cachemanager.h"
#include "filesystem.h"
#include <system_error>
#include <utility>
#include <unistd.h>
#include <thread>

Resource Storage::GetResource(const std::string &path)
{
    auto item = CacheManager::GetItem(path);
    if(!item)
    {
        try {
            char* cwd = NULL;
            cwd = getcwd(0, 0);
            std::string cur_dir(cwd);
            free(cwd);
            Resource res(path, IO::FileSystem::ReadFile(cur_dir + path));
            std::thread add_to_cache(CacheManager::PutItem, std::make_pair(path, res));
            add_to_cache.detach();

            return res;
        }
        catch(IO::fs_error &ex) {
            throw 404;
        }
        catch(std::system_error &ex) {
            throw 500;
        }
    }
    return item;
}
