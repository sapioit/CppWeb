#ifndef SETTINGS_H
#define SETTINGS_H
#include <string>


struct Settings {
    Settings();
    ~Settings() = default;
    Settings(const Settings&) = default;
    Settings(Settings&&) = default;
    Settings& operator=(const Settings&) = default;

    std::string root_path;
    int max_connections;
};

#endif // SETTINGS_H
