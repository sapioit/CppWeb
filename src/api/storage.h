#ifndef STORAGE_H
#define STORAGE_H

#include <vector>
#include <string>
#include "resource.h"
#include "outputscheduler.h"
#include <memory>
class Storage
{
    static std::unique_ptr<IO::OutputScheduler> OutScheduler;
public:
    static Resource GetResource(const std::string& path);

    static void InitializeOutputScheduler(int max_events);
    static IO::OutputScheduler& output_scheduler();
};

#endif // STORAGE_H
