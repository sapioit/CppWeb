#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include <http/response.h>
#include <http/request.h>
#include <io/socket.h>

class ResponseBuilder {
public:
  static void Respond(Http::Response, IO::Socket&);
};

#endif // RESPONSEMANAGER_H
