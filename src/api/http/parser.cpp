//
// Created by vladimir on 09.08.2015.
//

#include <http/components.h>
#include <http/parser.h>
#include <misc/log.h>

#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

using namespace Http;
constexpr auto space = ' ';
constexpr auto token = ':';
constexpr auto CRLF = "\r\n";

std::vector<Components::ContentType> GetAcceptedEncodings(
        const std::string& line) {
    return std::vector<Components::ContentType>();
}

Components::ContentType GetMimeType(const std::string& line) {
    // TODO parse line and get it
    return Components::ContentType::ApplicationJson;
}

Request Parser::operator()() {
    try {
        auto request = Init();
        request.header = std::move(GetHeader());
        return request;
    } catch (std::runtime_error& ex) {
        Log::e(ex.what());
        throw std::runtime_error("Could not parse request");
    }
    catch (IO::Socket::connection_closed_by_peer) {
        throw;
    }
}

Request Parser::Init() {
    try {

        auto line = _sock.ReadUntil(CRLF);
        if (line == "")
            throw std::runtime_error("Malformed request");

        Request request;
        auto first_space = line.find(space);
        try {
            request.method =
                    GetMethod(std::string(line.begin(), line.begin() + first_space));
        } catch (std::runtime_error& ex) {
            throw;
        }
        try {
            auto _URI_begin = line.find_first_not_of(space, first_space);
            auto _URI_end = line.find_first_of(space, _URI_begin + 1);
            auto _version_begin = line.find_first_not_of(space, _URI_end);
            auto _version_end = line.find_first_of("\r\n", _version_begin + 1);
            request.URI =
                    std::string(line.begin() + _URI_begin, line.begin() + _URI_end);
            std::string http_version_str(line.begin() + _version_begin,
                                         line.begin() + _version_end);
            auto dot = http_version_str.find('.');
            std::string version(http_version_str.begin() + dot - 1,
                                http_version_str.end());
            float version_f = std::stof(version);
            request.version = version_f;
            return request;
        } catch (std::exception& ex) {
            throw;
        }
    }
    catch(IO::Socket::connection_closed_by_peer) {
        throw;
    }
}

Components::Method Parser::GetMethod(const std::string& str) {
    auto method = Http::Components::methods.find(str);
    if (method == Http::Components::methods.end())
        throw std::runtime_error("Method not found");
    return method->second;
}

Header Parser::GetHeader() {
    Header header;
    std::size_t line_begin = 0;
    while (true) {
        auto line = _sock.ReadUntil(CRLF);
        if (line.size() == 2)
            break;
        if (line.empty())
            throw std::runtime_error("Bad request");

        auto key_begin = line_begin;
        auto key_end = line.find_first_of(token, line_begin);

        auto value_begin = line.find_first_not_of(space, key_end + 1);
        auto value_end = line.size() - 2;

        auto key = std::string(line.begin() + key_begin, line.begin() + key_end);
        auto value =
                std::string(line.begin() + value_begin, line.begin() + value_end);

        header.fields[key] = std::move(value);
    }
    header.accepted = GetAcceptedEncodings(header.fields[Header::Fields::Accept]);
    header.mime_type = GetMimeType(header.fields[Header::Fields::Content_Type]);
    return header;
}

Components::ContentType Parser::GetMimeTypeByExtension(const std::string& URI) {
    auto dot = URI.find_last_of('.');
    std::string ext(URI.begin() + dot + 1, URI.end());

    using namespace Components;

    if (ext == "png")
        return ContentType::ImagePng;
    if (ext == "jpg")
        return ContentType::ImageJpeg;
    if (ext == "mp4")
        return ContentType::MovieMp4;
    if (ext == "html" || ext == "htm")
        return ContentType::TextHtml;
    if (ext == "json")
        return ContentType::ApplicationJson;

    return ContentType::TextPlain;
}

std::string Parser::StripRoute(const std::string& URI) {
    auto firstSlash = URI.find_first_of('/');
    return { URI.begin() + firstSlash, URI.end() };
}

std::vector<std::string> Parser::Split(std::string source, char delimiter) {
    std::vector<std::string> result;
    std::istringstream ss(source); // Turn the string into a stream.
    std::string tok;

    while (std::getline(ss, tok, delimiter)) {
        if (!tok.empty())
            result.push_back(tok);
    }
    return result;
}
