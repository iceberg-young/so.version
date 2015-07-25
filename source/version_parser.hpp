#ifndef INCLUDE_SO_VERSION_PARSER_ONCE_FLAG
#define INCLUDE_SO_VERSION_PARSER_ONCE_FLAG

namespace so {
    class version_parser {
     public:
        version_parser(const std::string& version);

     public:
        bool on(char c) {
            return *this->cursor == c;
        }

     public:
        void expect_dot();

        version::ids_t get_ids();

        size_t get_number();

     private:
        const char* cursor;
    };
}

#endif//INCLUDE_SO_VERSION_PARSER_ONCE_FLAG
