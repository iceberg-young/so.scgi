/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */

#include "scgi.hpp"
#include <sstream>

namespace so {
    namespace {
        void append(std::string& p, const std::string& k, const std::string& v) {
            (p += k) += char(0);
            (p += v) += char(0);
        }

        std::string fragment(const char*& p) {
            std::string f = p;
            p += f.size();
            return f;
        }
    }

    std::string scgi_client::encode(size_t content) const {
        size_t hl = sizeof("CONTENT_LENGTH SCGI 1");
        std::string cl = std::to_string(content);
        hl += cl.size() + 1;
        for (auto& h : *this) {
            hl += h.first.size() + 1;
            hl += h.second.size() + 1;
        }

        std::string p = std::to_string(hl) + ':';
        p.reserve(p.size() + hl + 1); // +','
        append(p, "CONTENT_LENGTH", cl);
        append(p, "SCGI", "1");
        for (auto& h : *this) {
            append(p, h.first, h.second);
        }
        return p += ',';
    }

    void scgi_server::decode(const std::string& payload) {
        // headers length
        const char* p = payload.data();
        char* e = nullptr;
        size_t hl = std::strtoul(p, &e, 10);
        if (*e != ':') {
            throw scgi_payload_error{"Delimiter ':' is expected."};
        }
        const char* end = ++e + hl;

        // headers
        for (p = e; p < end; ++p) {
            std::string k = fragment(p);
            (*this)[k] = fragment(++p);
        }
        if (p != end or *p != ',') {
            throw scgi_payload_error{"Broken SCGI payload."};
        }
        this->content = ++p;

        // SCGI
        auto scgi = this->find("SCGI");
        if (scgi == this->end()) {
            throw scgi_payload_error{"Invalid SCGI payload."};
        }
        else if (scgi->second != "1") {
            throw scgi_payload_error{
              "Unsupported protocol version: " + scgi->second
            };
        }
        this->erase(scgi);

        // CONTENT_LENGTH
        auto cl = this->find("CONTENT_LENGTH");
        if (cl == this->end()) {
            throw scgi_payload_error{
              "The \"CONTENT_LENGTH\" header must always be present,"
                " even if its value is \"0\"."
            };
        }
        this->content_length = std::strtoul(cl->second.data(), &e, 10);
        if (*e != 0) {
            throw scgi_payload_error{"Invalid content length."};
        }
        this->erase(cl);
    }
}
