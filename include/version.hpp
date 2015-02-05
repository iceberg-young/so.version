#ifndef INCLUDE_SO_VERSION_ONCE_FLAG
#define INCLUDE_SO_VERSION_ONCE_FLAG

#include <string>

// See <sys/sysmacros.h>, so you know how horrible C is!
#undef major
#undef minor

namespace so {
    // Representation of http://semver.org/ described object.
    class version
    {
    public:
        enum class change
        {
            internal,
            compatible,
            incompatible,

            fixed = internal,
            deprecated = compatible,
        };

    public:
        version(
          unsigned major = 0,
          unsigned minor = 0,
          unsigned patch = 0,
          const std::string& label = "",
          const std::string& build = ""
        ) :
          major(major),
          minor(minor),
          patch(patch),
          label(label),
          build(build) {
        }

    public:
        static version parse(const std::string& text);

    public:
        bool is_stable() const {
            return this->major > 0 and this->label.empty();
        }

    public:
        version next_major() const {
            return version{this->major + 1, 0, 0, "", this->build};
        }

        version next_minor() const {
            return version{this->major, this->minor + 1, 0, "", this->build};
        }

        version next_patch() const {
            return version{this->major, this->minor, this->patch + 1, "", this->build};
        }

        version next_pre() const;

    public:
        version final() const {
            return version{this->major, this->minor, this->patch, "", this->build};
        }

        version general() const {
            return version{this->major, this->minor, this->patch, this->label, ""};
        }

    public:
        version operator+(change type) const;

    public:
        bool operator==(const version& other) const {
            return this->major == other.major
              and this->minor == other.minor
              and this->patch == other.patch
              and this->label == other.label;
            // `build` should be ignored.
        }

        bool operator==(const std::string& other) const {
            return *this == version::parse(other);
        }

        bool operator!=(const version& other) const {
            return !(*this == other);
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
        const unsigned major;

        // When you add functionality in a backwards-compatible manner.
        const unsigned minor;

        // When you make backwards-compatible bug fixes.
        const unsigned patch;

        // For pre-release.
        const std::string label;

        // For build metadata.
        const std::string build;
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
}

#endif//INCLUDE_SO_VERSION_ONCE_FLAG
