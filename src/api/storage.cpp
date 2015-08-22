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
    char* cwd = NULL;
    cwd = getcwd(0, 0);
    std::string cur_dir(cwd);
    free(cwd);
    std::string fpath(cur_dir + path);

    auto item = CacheManager::GetItem(fpath);

    if(item)
    {
        struct stat st;
        auto st_res = stat(fpath.c_str(), &st);
        if(st_res != 0) {
            //TODO remove item from cache
            throw 404;
        }
        else {
            if(st.st_mtime > item.stat().st_mtime) {
                try {
                    Resource res(fpath);
                    CacheManager::ReplaceItem(fpath, res);
                    return res;
                }
                catch(IO::fs_error& ex) {
                    throw 404;
                }
                catch(std::system_error &ex) {
                    throw 500;
                }
            }
            else return item;
        }
    }
    else
    {
        try {
            Resource res(fpath);
            CacheManager::PutItem(std::make_pair(fpath, res));
            return res;
        }
        catch(IO::fs_error &ex) {
            throw 404;
        }
        catch(std::system_error &ex) {
            throw 500;
        }
    }
}

void Storage::InitializeOutputScheduler(int max_events)
{
    Storage::OutScheduler.reset(new IO::OutputScheduler(max_events));
}

IO::OutputScheduler &Storage::output_scheduler()
{
    return (*Storage::OutScheduler);
}
