/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */
#pragma once

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
            internal = 1,
            compatible = 2,
            incompatible = 4,
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
        version release() const {
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

    inline version::change operator|(version::change a, version::change b) {
        return static_cast<version::change>(
          static_cast<int>(a) | static_cast<int>(b)
        );
    }

    inline version::change operator^(version::change a, version::change b) {
        return static_cast<version::change>(
          static_cast<int>(a) ^ static_cast<int>(b)
        );
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
