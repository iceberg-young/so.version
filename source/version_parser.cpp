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

    version_parser::version_parser(literal_t& version) :
      cursor(version) {
        char c = *this->cursor;
        while (c and (c < '0' or c > '9')) {
            c = *++this->cursor;
        }
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
        auto begin = &*this->cursor;
        char* end = nullptr;
        auto value = strtoul(begin, &end, 10);
        if (begin == end) {
            throw version_parse_error{
              "Digit is expected, but got char(" + std::to_string(*this->cursor) + ")."
            };
        }
        size_t length = end - begin;
        if (value == std::numeric_limits<decltype(value)>::max()) {
            std::string number{begin, length};
            throw version_parse_error{
              number + " is too big (" + std::to_string(length) + " digits)."
            };
        }
        this->cursor += length;
        return value;
    }
}
