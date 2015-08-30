#ifndef RESPONSE_H
#define RESPONSE_H

#include <http/header.h>
#include <http/request.h>
#include <json/json.h>
#include <misc/resource.h>

#include <string>
#include <iostream>

namespace Http {
class Response {
public:
  Response();
  Response(const Request&);
  Response(const Request&, int);
  Response(const Request&, const std::string&);
  Response(const Request&, int, const std::string&);
  Response(const Request&, const Resource&);
  Response(const Request&, const Json::Value&);
  Response(const Response&) = default;
  Response(Response&&) = default;
  Response& operator=(const Response&) = default;
  ~Response() = default;

  bool should_cache() const;
  std::uint32_t get_expiry() const;
  bool should_close() const;
  bool has_body() const;
  bool has_resource() const;
  bool is_error() const;

  int code() const;
  void setCode(int code);

  const Components::ContentType& getContent_type() const;
  void setContent_type(const Components::ContentType& value);

  const Resource& getResource() const;
  void setResource(const Resource& resource);

  std::string str_header() const;
  const Request& getRequest() const;

  std::string getText() const;
  void setText(const std::string& text);

private:
  Request _request;
  Resource _resource;
  int _code;
  std::string _text;
  Components::ContentType _content_type = Components::ContentType::TextPlain;
};
};

#endif // RESPONSE_H
