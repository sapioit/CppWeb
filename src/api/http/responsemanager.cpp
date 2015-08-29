#include <http/responsemanager.h>
#include <io/outputscheduler.h>
#include <misc/log.h>
#include <misc/storage.h>

#include <sstream>

void ResponseManager::Respond(Http::Response response, IO::Socket& socket) {
  try {
    auto raw_response = response.str();
    IO::OutputScheduler::get().ScheduleWrite(socket, std::move(raw_response));
  } catch (std::exception& ex) {
    Log::e(ex.what());
    ResponseManager::Respond({ response.getRequest(), 500 }, socket);
  }
}
