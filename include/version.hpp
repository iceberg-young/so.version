#ifndef INCLUDE_SO_VERSION_ONCE_FLAG
#define INCLUDE_SO_VERSION_ONCE_FLAG

#include <list>
#include <string>
#include <stdexcept>

// See <sys/sysmacros.h>, so you know how horrible C is!
#undef major
#undef minor

namespace so {
    // Representation of http://semver.org/ described object.
    class version {
     public:
        enum class change {
            internal,
            compatible,
            incompatible,

            fixed = internal,
            deprecated = compatible,
        };

        using ids_t = std::list<std::string>;

     public:
        version(
          size_t major = 0,
          size_t minor = 0,
          size_t patch = 0,
          const ids_t& label = {},
          const ids_t& build = {}
        ) :
          major(major),
          minor(minor),
          patch(patch),
          label(label),
          build(build) {}

     public:
        static version parse(std::string::const_iterator& iterator);

        static version parse(const std::string& text) {
            auto iterator = text.begin();
            return version::parse(iterator);
        }

     public:
        version next_major() const {
            return version{this->major + 1, 0, 0, {}, this->build};
        }

        version next_minor() const {
            return version{this->major, this->minor + 1, 0, {}, this->build};
        }

        version next_patch() const {
            return version{
              this->major, this->minor, this->patch + 1, {}, this->build
            };
        }

     public:
        version final() const {
            return version{
              this->major, this->minor, this->patch, {}, this->build
            };
        }

        version general() const {
            return version{
              this->major, this->minor, this->patch, this->label, {}
            };
        }

     public:
        version operator+(change type) const;

     public:
        bool operator==(const version& other) const {
            return this->major == other.major
              and this->minor == other.minor
              and this->patch == other.patch
              and this->label == other.label;
            // Build metadata does not figure into precedence.
        }

        bool operator==(const std::string& other) const {
            return *this == version::parse(other);
        }

        bool operator!=(const version& other) const {
            return not (*this == other);
        }

        bool operator!=(const std::string& other) const {
            return *this != version::parse(other);
        }

        bool operator<(const version& other) const;

        bool operator<(const std::string& other) const {
            return *this < version::parse(other);
        }

        bool operator<=(const version& other) const {
            return *this == other or *this < other;
        }

        bool operator<=(const std::string& other) const {
            return *this <= version::parse(other);
        }

        bool operator>(const version& other) const {
            return other < *this;
        }

        bool operator>(const std::string& other) const {
            return *this > version::parse(other);
        }

        bool operator>=(const version& other) const {
            return other <= *this;
        }

        bool operator>=(const std::string& other) const {
            return *this >= version::parse(other);
        }

     public:
        std::string to_string() const;

        operator std::string() const {
            return this->to_string();
        }

     public:
        // When you make incompatible API changes.
        const size_t major;

        // When you add functionality in a backwards-compatible manner.
        const size_t minor;

        // When you make backwards-compatible bug fixes.
        const size_t patch;

        // For pre-release.
        const ids_t label;

        // For build metadata.
        const ids_t build;
    };

    inline bool operator==(const std::string& a, const version& b) {
        return version::parse(a) == b;
    }

    inline bool operator!=(const std::string& a, const version& b) {
        return version::parse(a) != b;
    }

    inline bool operator<(const std::string& a, const version& b) {
        return version::parse(a) < b;
    }

    inline bool operator<=(const std::string& a, const version& b) {
        return version::parse(a) <= b;
    }

    inline bool operator>(const std::string& a, const version& b) {
        return version::parse(a) > b;
    }

    inline bool operator>=(const std::string& a, const version& b) {
        return version::parse(a) >= b;
    }

    class version_parse_error :
      public std::domain_error {
     public:
        version_parse_error(const std::string& what) :
          domain_error(what) {}
    };

    namespace is {
        inline bool stable(const version& v) {
            return v.major > 0 and v.label.empty();
        }
    }
}

#endif//INCLUDE_SO_VERSION_ONCE_FLAG
