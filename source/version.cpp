#include "version.hpp"
#include "parser.hpp"

namespace so {
    namespace {
        long numeric(const std::string& id) {
            char* end = nullptr;
            long v = strtoul(id.c_str(), &end, 10);
            return end != &*id.end() ? -1 : v;
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

        bool operator<(const version::ids_t& a, const version::ids_t& b) {
            // A pre-release version has lower precedence than a normal version.
            if (b.empty()) return not a.empty();

            auto ai = a.begin();
            auto ae = a.end();
            auto bi = b.begin();
            auto be = b.end();
            for (; ai != ae and bi != be; ++ai, ++bi) {
                if (*ai == *bi) continue;

                // Numeric identifiers always have lower precedence than non-numeric identifiers.
                int av = numeric(*ai);
                int bv = numeric(*bi);
                return (av != -1)
                  ? (bv != -1 ? av < bv : true)
                  : (bv != -1 ? false : *ai < *bi);
            }

            // A larger set has a higher precedence than a smaller set,
            // if all of the preceding identifiers are equal.
            return bi != be;
        }
    }

    version version::parse(const std::string& text) {
        auto p = text.c_str();
        version_parser parser(p);
        size_t major = parser.number();
        parser.dot();
        size_t minor = parser.number();
        parser.dot();
        size_t patch = parser.number();
        ids_t label, build;
        if (parser.on('-')) {
            label = parser.ids();
        }
        if (parser.on('+')) {
            build = parser.ids();
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
        };
    }

    bool version::operator<(const version& other) const {
        return this->major == other.major
          ? (this->minor == other.minor
            ? (this->patch == other.patch
              ? this->label < other.label
              : this->patch < other.patch)
            : this->minor < other.minor)
          : this->major < other.major;
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
