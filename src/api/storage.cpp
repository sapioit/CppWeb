#include "storage.h"
#include "cachemanager.h"
#include "filesystem.h"
#include <system_error>
#include <utility>
#include <thread>


Settings Storage::_settings;

const Settings& Storage::settings() {
  return Storage::_settings;
}

void Storage::setSettings(const Settings& settings) {
  Storage::_settings = settings;
}
