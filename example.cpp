#include "include/scgi.hpp"
#include <iostream>

using namespace so;
using namespace std;

int main() {
    scgi_client c{
      {"hello", "world"}
    };
    cout << c.encode(sizeof(__PRETTY_FUNCTION__)) << __PRETTY_FUNCTION__ << endl;

    constexpr char payload[]{
      "37:"
        "CONTENT_LENGTH\0" "11\0"
        "SCGI\0" "1\0"
        "hello\0" "world\0"
        "," "int main()"
    };
    scgi_server s{std::string{payload, sizeof(payload)}};
    for (auto& h : s) {
        cout << h.first << " : " << h.second << endl;
    }
    return 0;
}
