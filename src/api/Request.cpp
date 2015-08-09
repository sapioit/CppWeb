//
// Created by vladimir on 08.08.2015.
//

#include <ostream>
#include "Request.h"

const std::map<std::string, Http::Request::Method> Http::Request::_methods{
        std::make_pair(std::string("GET"), Http::Request::Method::Get),
        std::make_pair("POST", Http::Request::Method::Post),
        std::make_pair("PUT", Http::Request::Method::Put),
        std::make_pair("DELETE", Http::Request::Method::Delete),
        std::make_pair("Connect", Http::Request::Method::Connect),
        std::make_pair("HEAD", Http::Request::Method::Head),
        std::make_pair("OPTIONS", Http::Request::Method::Options),
        std::make_pair("TRACE", Http::Request::Method::Trace),
        std::make_pair("COPY", Http::Request::Method::Copy),
        std::make_pair("LOCK", Http::Request::Method::Lock),
        std::make_pair("MKCOL", Http::Request::Method::Mkcol),
        std::make_pair("MOVE", Http::Request::Method::Move),
        std::make_pair("PROPFIND", Http::Request::Method::Propfind),
        std::make_pair("PROPPATCH", Http::Request::Method::Proppatch),
        std::make_pair("UNLOCK", Http::Request::Method::Unlock),
        std::make_pair("REPORT", Http::Request::Method::Report),
        std::make_pair("MKACTIVITY", Http::Request::Method::Mkactivity),
        std::make_pair("CHECKOUT", Http::Request::Method::Checkout),
        std::make_pair("MERGE", Http::Request::Method::Merge),
        std::make_pair("M-SEARCH", Http::Request::Method::M_Search),
        std::make_pair("NOTIFY", Http::Request::Method::Notify),
        std::make_pair("SUBSCRIBE", Http::Request::Method::Subscribe),
        std::make_pair("UNSUBSCRIBE", Http::Request::Method::Unsubscribe)
};


bool Http::Request::operator==(const Http::Request & other)
{
    bool bmethod = method == other.method;
    bool buri = URI == other.URI;
    bool bversion = version == other.version;
    bool bheader = header == other.header;
    bool bbody = body == other.body;
    return (bmethod && buri && bversion && bheader && bbody);
}

