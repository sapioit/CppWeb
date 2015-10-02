#ifndef STORAGE_H
#define STORAGE_H

#include "settings.h"

class Storage {
  static Settings _settings;

 public:
  static const Settings& settings();
  static void setSettings(const Settings& settings);
};

#endif  // STORAGE_H
