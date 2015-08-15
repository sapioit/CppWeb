#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "resource.h"
class Storage
{
public:
    static Resource GetResource(const std::string& path);
};

#endif // STORAGE_H
