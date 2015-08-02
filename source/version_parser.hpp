/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */
#pragma once

namespace so {
    class version_parser {
     public:
        using literal_t = std::string::const_iterator;

        version_parser(literal_t& version);

     public:
        bool on(char c) {
            return *this->cursor == c;
        }

     public:
        void expect_dot();

        version::ids_t get_ids();

        size_t get_number();

     private:
        literal_t& cursor;
    };
}
