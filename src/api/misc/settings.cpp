#include <misc/settings.h>
#include <io/filesystem.h>

Settings::Settings()
    : root_path(IO::FileSystem::GetCurrentDirectory()), max_connections(1000) {}
