/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */
#pragma once

#include <map>
#include <string>
#include <stdexcept>

namespace so {
    namespace scgi {
        using headers_t = std::map<std::string, std::string>;

        // Header name specified in RFC 2616
        constexpr char request_uri[]{"REQUEST_URI"};

        // Header names specified in RFC 3875
        constexpr char content_type[]{"CONTENT_TYPE"};
        constexpr char request_method[]{"REQUEST_METHOD"};
        constexpr char query_string[]{"QUERY_STRING"};

        constexpr char remote_address[]{"REMOTE_ADDR"};
        constexpr char remote_port[]{"REMOTE_PORT"};

        constexpr char server_name[]{"SERVER_NAME"};
        constexpr char server_port[]{"SERVER_PORT"};
        constexpr char server_protocol[]{"SERVER_PROTOCOL"};

        // Header names from unknown source
        constexpr char document_root[]{"DOCUMENT_ROOT"};
        //^ Part of PATH_TRANSLATED
        constexpr char document_uri[]{"DOCUMENT_URI"};
        //^ Alias of PATH_INFO
    }

    class scgi_client :
      public scgi::headers_t {
     public:
        scgi_client() {}

        scgi_client(std::initializer_list<scgi::headers_t::value_type> headers) :
          scgi::headers_t(headers) {}

     public:
        std::string encode(size_t content = 0) const;
    };

    class scgi_server :
      public scgi::headers_t {
     public:
        scgi_server() :
          content_length(0),
          content(nullptr) {}

        scgi_server(const std::string& payload) :
          scgi_server() {
            this->decode(payload);
        }

     public:
        void decode(const std::string& payload);

     public:
        size_t content_length;
        const char* content;
    };

    class scgi_payload_error :
      public std::domain_error {
     public:
        scgi_payload_error(const std::string& what) :
          domain_error(what) {}
    };
}
