#include "version.hpp"
#include "version_parser.hpp"
#include <limits>

namespace so {
    namespace {
        const auto invalid_number = std::numeric_limits<size_t>::max();

        size_t get_number(const std::string& id) {
            char* end = nullptr;
            auto value = strtoul(id.c_str(), &end, 10);
            static_assert(
              sizeof(size_t) == sizeof(decltype(value)),
              "The max value will be used to check overflow, so "
                "return type of `strtoul` must has the same size as `size_t`."
            );
            return end == &*id.end() ? value : invalid_number;
        }

        bool operator<(const version::ids_t& a, const version::ids_t& b) {
            // A pre-release version has lower precedence than a normal version.
            if (a.empty()) return false;
            if (b.empty()) return true;

            auto ai = a.begin(), ae = a.end();
            auto bi = b.begin(), be = b.end();
            for (; ai != ae and bi != be; ++ai, ++bi) {
                if (*ai == *bi) continue;

                // Numeric identifiers always have lower precedence than non-numeric identifiers.
                auto an = get_number(*ai);
                auto bn = get_number(*bi);
                return (an == invalid_number)
                  ? (bn == invalid_number ? *ai < *bi : false)
                  : (bn == invalid_number ? true : an < bn);
            }

            // A larger set has a higher precedence than a smaller set,
            // if all of the preceding identifiers are equal.
            return bi != be;
        }
    }

    std::string& operator+=(std::string& target, const version::ids_t& ids) {
        auto i = ids.begin();
        auto e = ids.end();
        target += *i;
        while (++i != e) {
            target += '.' + *i;
        }
        return target;
    }

    version version::parse(std::string::const_iterator& iterator) {
        version_parser parser{iterator};
        size_t major = parser.get_number();
        parser.expect_dot();
        size_t minor = parser.get_number();
        parser.expect_dot();
        size_t patch = parser.get_number();
        ids_t label, build;
        if (parser.on('-')) {
            label = parser.get_ids();
        }
        if (parser.on('+')) {
            build = parser.get_ids();
        }
        return version{major, minor, patch, label, build};
    }

    version version::operator+(change type) const {
        switch (type) {
            case change::internal: {
                return this->next_patch();
            }
            case change::compatible: {
                return this->next_minor();
            }
            case change::incompatible: {
                return this->next_major();
            }
        }
        throw type;
    }

    bool version::operator<(const version& other) const {
        if (this->major != other.major) {
            return this->major < other.major;
        }
        if (this->minor != other.minor) {
            return this->minor < other.minor;
        }
        if (this->patch != other.patch) {
            return this->patch < other.patch;
        }
        return this->label < other.label;
        // Build metadata does not figure into precedence.
    }

    std::string version::to_string() const {
        auto literal = std::to_string(this->major)
          + '.' + std::to_string(this->minor)
          + '.' + std::to_string(this->patch);
        if (not this->label.empty()) {
            literal += '-';
            literal += this->label;
        }
        if (not this->build.empty()) {
            literal += '+';
            literal += this->build;
        }
        return literal;
    }
}
