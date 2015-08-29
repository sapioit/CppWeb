#ifndef COMPONENTS
#define COMPONENTS
#include <map>
namespace Http {
namespace Components {

enum class Method {
  Delete,
  Get,
  Head,
  Post,
  Put,
  Connect,
  Options,
  Trace,
  Copy,
  Lock,
  Mkcol,
  Move,
  Propfind,
  Proppatch,
  Unlock,
  Report,
  Mkactivity,
  Checkout,
  Merge,
  M_Search,
  Notify,
  Subscribe,
  Unsubscribe
};

const std::map<const std::string, const Method> methods{
  std::make_pair("GET", Method::Get), std::make_pair("POST", Method::Post),
  std::make_pair("PUT", Method::Put), std::make_pair("DELETE", Method::Delete),
  std::make_pair("Connect", Method::Connect),
  std::make_pair("HEAD", Method::Head),
  std::make_pair("OPTIONS", Method::Options),
  std::make_pair("TRACE", Method::Trace), std::make_pair("COPY", Method::Copy),
  std::make_pair("LOCK", Method::Lock), std::make_pair("MKCOL", Method::Mkcol),
  std::make_pair("MOVE", Method::Move),
  std::make_pair("PROPFIND", Method::Propfind),
  std::make_pair("PROPPATCH", Method::Proppatch),
  std::make_pair("UNLOCK", Method::Unlock),
  std::make_pair("REPORT", Method::Report),
  std::make_pair("MKACTIVITY", Method::Mkactivity),
  std::make_pair("CHECKOUT", Method::Checkout),
  std::make_pair("MERGE", Method::Merge),
  std::make_pair("M-SEARCH", Method::M_Search),
  std::make_pair("NOTIFY", Method::Notify),
  std::make_pair("SUBSCRIBE", Method::Subscribe),
  std::make_pair("UNSUBSCRIBE", Method::Unsubscribe)
};

enum class ContentType {
  ApplicationJson,
  TextPlain,
  TextHtml,
  ImageJpeg,
  ImagePng,
  MovieMp4
};

const std::map<const ContentType, const std::string> content_types{
  std::make_pair(ContentType::ApplicationJson, "application/json"),
  std::make_pair(ContentType::TextPlain, "text/plain"),
  std::make_pair(ContentType::TextHtml, "text/html; charset=utf-8"),
  std::make_pair(ContentType::ImageJpeg, "image/jpeg"),
  std::make_pair(ContentType::ImagePng, "image/png"),
  std::make_pair(ContentType::MovieMp4, "video/mp4")
};

enum class StatusCode {
  OK = 200,
  BadRequest = 400,
  NotFound = 404,
  UnsupportedMediaType = 415,
  InternalServerError = 500
};

const std::map<const StatusCode, const std::string> status_codes{
  std::make_pair(StatusCode::OK, "OK"),
  std::make_pair(StatusCode::BadRequest, "Bad Request"),
  std::make_pair(StatusCode::NotFound, "Not Found"),
  std::make_pair(StatusCode::UnsupportedMediaType, "Unsupported Media Type"),
  std::make_pair(StatusCode::InternalServerError, "Internal Server Error")
};
}
}
#endif // COMPONENTS
