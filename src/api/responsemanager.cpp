#include "responsemanager.h"
#include <sstream>
#include "log.h"
#include "storage.h"
#include "outputscheduler.h"

void ResponseManager::Respond(Http::Response response, IO::Socket& socket) {
  try {
    auto raw_response = response.str();
    IO::OutputScheduler::get().ScheduleWrite(socket, std::move(raw_response));
  } catch (std::exception& ex) {
    Log::e(ex.what());
    ResponseManager::Respond({response.getRequest(), 500}, socket);
  }
}
