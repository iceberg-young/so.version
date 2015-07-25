#include "version.hpp"
#include "version_parser.hpp"
#include <limits>

namespace so {
    namespace {
        bool is_id(char c) {
            return (c >= '0' and c <= '9')
              or (c >= 'A' and c <= 'Z')
              or (c >= 'a' and c <= 'z')
              or (c == '-');
        }
    }

    version_parser::version_parser(const std::string& version) {
        auto p = version.data();
        while (*p and (*p < '0' or *p > '9')) {++p;}
        this->cursor = p;
    }

    void version_parser::expect_dot() {
        if (*this->cursor != '.') {
            throw version_parse_error{
              "Dot (.) is expected, but got char(" + std::to_string(*this->cursor) + ")."
            };
        }
        ++this->cursor;
    }

    version::ids_t version_parser::get_ids() {
        version::ids_t ids;
        while (*++this->cursor) {
            std::string id;
            while (is_id(*this->cursor)) {
                id += *this->cursor++;
            }
            if (id.empty()) {
                throw version_parse_error{"Identifier MUST NOT be empty."};
            }
            ids.push_back(std::move(id));
            if (*this->cursor != '.') break;
        }
        return ids;
    }

    size_t version_parser::get_number() {
        char* next = nullptr;
        auto value = strtoul(this->cursor, &next, 10);
        if (this->cursor == next) {
            throw version_parse_error{
              "Digit is expected, but got char(" + std::to_string(*this->cursor) + ")."
            };
        }
        if (value == std::numeric_limits<decltype(value)>::max()) {
            size_t length = next - this->cursor;
            std::string number{this->cursor, length};
            throw version_parse_error{
              number + " is too big (" + std::to_string(length) + " digits)."
            };
        }
        this->cursor = next;
        return value;
    }
}
