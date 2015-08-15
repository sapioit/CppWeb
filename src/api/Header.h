//
// Created by vladimir on 08.08.2015.
//

#ifndef SOCKET_HEADER_H
#define SOCKET_HEADER_H

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

namespace Http {
    struct Header {
        Header() = default;
        Header(const Header &) = default;
        Header(Header &&) = default;
        bool operator==(const Header& other) { return fields == other.fields; }
        Header& operator=(const Header&) = default;
        ~Header() = default;

        enum class MIME_Type {
            ApplicationJson
        };

        MIME_Type mime_type;
        std::vector<MIME_Type> accepted;

        std::map<std::string, std::string> fields;

        //Common
        struct Fields {
            constexpr static auto Permanent = "Permanent";
            constexpr static auto If_Modified_Since = "If-Modified-Since";
            constexpr static auto Cache_Control = "Cache-Control";
            constexpr static auto Content_Length = "Content-Length";
            constexpr static auto Content_MD5 = "Content-MD5";
            constexpr static auto Content_Type = "Content-Type";
            constexpr static auto Date = "Date";
            constexpr static auto Pragma = "Pragma";
            constexpr static auto Upgrade = "Upgrade";
            constexpr static auto Via = "Via";
            constexpr static auto Warning = "Warning";

            //Request
            constexpr static auto Accept = "Accept";
            constexpr static auto Accept_Charset = "Accept-Charset";
            constexpr static auto Accept_Encoding = "Accept-Encoding";
            constexpr static auto Accept_Language = "Accept-Language";
            constexpr static auto Accept_Datetime = "Accept-Datetime";
            constexpr static auto Authorization = "Authorization";
            constexpr static auto Connection = "Connection";
            constexpr static auto Cookie = "Cookie";
            constexpr static auto Expect = "Expect";
            constexpr static auto From = "From";
            constexpr static auto Host = "Host";
            constexpr static auto If_Match = "If-Match";
            constexpr static auto If_None_Match = "If-None-Match";
            constexpr static auto If_Range = "If-Range";
            constexpr static auto If_Unmodified_Since = "If-Unmodified-Since";
            constexpr static auto Max_Forwards = "Max-Forwards";
            constexpr static auto Origin = "Origin";
            constexpr static auto Proxy_Authorization = "Proxy-Authorization";
            constexpr static auto Range = "Range";
            constexpr static auto Referer = "Referer";
            constexpr static auto TE = "TE";
            constexpr static auto User_Agent = "User-Agent";
//non-standard
            constexpr static auto X_Requested_With = "X-Requested-With";
            constexpr static auto DNT = "DNT";
            constexpr static auto X_Forwarded_For = "X-Forwarded-For";
            constexpr static auto X_Forwarded_Host = "X-Forwarded-Host";
            constexpr static auto X_Forwarded_Proto = "X-Forwarded-Proto";
            constexpr static auto Front_End_Https = "Front-End-Https";
            constexpr static auto X_Http_Method_Override = "X-Http-Method-Override";
            constexpr static auto X_ATT_DeviceId = "X-ATT-DeviceId";
            constexpr static auto X_Wap_Profile = "X-Wap-Profile";
            constexpr static auto Proxy_Connection = "Proxy-Connection";
            constexpr static auto X_UIDH = "X-UIDH";
            constexpr static auto X_Csrf_Token = "X-Csrf-Token";

//Response
            constexpr static auto Access_Control_Allow_Origin = "Access-Control-Allow-Origin";
            constexpr static auto Accept_Patch = "Accept-Patch";
            constexpr static auto Accept_Ranges = "Accept-Ranges";
            constexpr static auto Age = "Age";
            constexpr static auto Allow = "Allow";
            constexpr static auto Content_Disposition = "Content-Disposition";
            constexpr static auto Content_Encoding = "Content-Encoding";
            constexpr static auto Content_Language = "Content-Language";
            constexpr static auto Content_Location = "Content-Location";
            constexpr static auto Content_Range = "Content-Range";
            constexpr static auto ETag = "ETag";
            constexpr static auto Expires = "Expires";
            constexpr static auto Last_Modified = "Last-Modified";
            constexpr static auto Link = "Link";
            constexpr static auto Location = "Location";
            constexpr static auto P3P = "P3P";
            constexpr static auto Proxy_Authenticate = "Proxy-Authenticate";
            constexpr static auto Public_Key_PinsPerma = "Public-Key-PinsPerma";
            constexpr static auto Retry_After = "Retry-After";
            constexpr static auto Server = "Server";
            constexpr static auto Set_Cookie = "Set-Cookie";
            constexpr static auto Status = "Status";
            constexpr static auto Strict_Transport_Security = "Strict-Transport-Security";
            constexpr static auto Trailer = "Trailer";
            constexpr static auto Transfer_Encoding = "Transfer-Encoding";
            constexpr static auto Vary = "Vary";
            constexpr static auto WWW_Authenticate = "WWW-Authenticate";
            constexpr static auto X_Frame_Options = "X-Frame-Options";
//non-standard
            constexpr static auto X_XSS_Protection = "X-XSS-Protection";
            constexpr static auto Content_Security_Policy = "Content-Security-Policy";
            constexpr static auto X_Content_Security_Policy = "X-Content-Security-Policy";
            constexpr static auto X_WebKit_CSP = "X-WebKit-CSP";
            constexpr static auto X_Content_Type_Options = "X-Content-Type-Options";
            constexpr static auto X_Powered_By = "X-Powered-By";
            constexpr static auto X_Content_Duration = "X-Content-Duration";
            constexpr static auto X_UA_Compatible = "X-UA-Compatible";
        };
    };
}


#endif //SOCKET_HEADER_H
