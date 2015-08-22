#include "storage.h"
#include "cachemanager.h"
#include "filesystem.h"
#include <system_error>
#include <utility>
#include <unistd.h>
#include <thread>

std::unique_ptr<IO::OutputScheduler> Storage::OutScheduler;


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
            std::string fpath(cur_dir + path);
            Resource res(path, IO::FileSystem::ReadFile(fpath));
            CacheManager::PutItem(std::make_pair(path, res));

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

void Storage::InitializeOutputScheduler(int max_events)
{
    Storage::OutScheduler.reset(new IO::OutputScheduler(max_events));
}

IO::OutputScheduler &Storage::output_scheduler()
{
    return (*Storage::OutScheduler);
}
