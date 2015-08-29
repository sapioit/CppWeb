#include "storage.h"

Settings Storage::_settings;

const Settings& Storage::settings() {
  return Storage::_settings;
}

void Storage::setSettings(const Settings& settings) {
  Storage::_settings = settings;
}
