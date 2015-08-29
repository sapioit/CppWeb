#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include "response.h"
#include "request.h"
#include "socket.h"

class ResponseManager {
public:
  static void Respond(Http::Response, IO::Socket&);
};

#endif // RESPONSEMANAGER_H
