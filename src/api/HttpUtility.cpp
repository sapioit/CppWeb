//
// Created by vladimir on 09.08.2015.
//

#include "HttpUtility.h"

using namespace Http;
constexpr auto space = ' ';
constexpr auto token = ':';
constexpr auto CRLF = "\r\n";

Request HttpUtility::ParseFrom(std::shared_ptr<IO::Socket> socket) {
    auto parse_init = [](std::shared_ptr<IO::Socket> socket) -> Request {
        /* Get method, URI and version */
        auto line = (*socket).ReadUntil(CRLF);
        if (line == "")
            return Request();

        auto get_method = [](const std::string &str) -> Request::Method {
            auto method = Http::Request::_methods.find(str);
            if (method == Http::Request::_methods.end())
                throw std::runtime_error("Method not found");
            return method->second;
        };

        Request request;
        auto first_space = line.find(space);
        try {
            request.method = get_method(std::string(line.begin(), line.begin() + first_space));
        }
        catch (std::runtime_error &ex) {
            throw;
        }

        auto _URI_begin = line.find_first_not_of(space, first_space);
        auto _URI_end = line.find_first_of(space, _URI_begin + 1);

        auto _version_begin = line.find_first_not_of(space, _URI_end);
        auto _version_end = line.find_first_of("\r\n", _version_begin + 1);

        auto _CRLF_begin = line.find_first_of('\r', _version_end);
        auto _CRLF_end = line.find_first_of('\n', _CRLF_begin + 1);

        request.URI = std::string(line.begin() + _URI_begin, line.begin() + _URI_end);
        request.version = std::string(line.begin() + _version_begin, line.begin() + _version_end);
        return request;
    };

    auto parse_header = [](std::shared_ptr<IO::Socket> socket) -> Header {
        Header header;
        std::size_t line_begin = 0;
        while (true) {
            auto line = (*socket).ReadUntil(CRLF);
            if (line.size() == 2)
                break;
            if (line.empty())
                throw std::runtime_error("Bad request");

            auto key_begin = line_begin;
            auto key_end = line.find_first_of(token, line_begin);

            auto value_begin = line.find_first_not_of(space, key_end + 1);
            auto value_end = line.size() - 2;

            auto key = std::string(line.begin() + key_begin, line.begin() + key_end);
            auto value = std::string(line.begin() + value_begin, line.begin() + value_end);

            header.fields[key] = std::move(value);
        }
        return header;
    };

    auto request = std::move(parse_init(socket));
    request.header = std::move(parse_header(socket));
    return request;
}
