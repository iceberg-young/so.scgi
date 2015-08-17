/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */
#pragma once

#include <map>
#include <string>
#include <stdexcept>

namespace so {
    using scgi_headers_t=std::map<std::string, std::string>;

    class scgi_client :
      public scgi_headers_t {
     public:
        scgi_client() {}

        scgi_client(std::initializer_list<scgi_headers_t::value_type> headers) :
          scgi_headers_t(headers) {}

     public:
        std::string encode(size_t content = 0) const;
    };

    class scgi_server :
      public scgi_headers_t {
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
