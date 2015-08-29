#include "settings.h"
#include "filesystem.h"

Settings::Settings()
    : root_path(IO::FileSystem::GetCurrentDirectory()), max_connections(1000) {}
