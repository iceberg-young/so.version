#include "version.hpp"

namespace so {
    version version::parse(const std::string& text) {
        return version{/* TODO */};
    }

    version version::next_pre() const {
        if (this->label.empty()) return *this;

        // find the 1st numeric identifier, then increase it
        // if none, append one more identifier: 1
        return version{/* TODO */};
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

    bool label_less(const std::string& a, const std::string& b) {
        // A pre-release version has lower precedence than a normal version.
        if (b.empty()) return !a.empty();

        return false; // TODO
    }

    bool version::operator<(const version& other) const {
        return this->major == other.major
          ? (this->minor == other.minor
            ? (this->patch == other.patch
              ? label_less(this->label, other.label)
              : this->patch < other.patch)
            : this->minor < other.minor)
          : this->major < other.major;
    }

    std::string version::to_string() const {
        auto literal = std::to_string(this->major)
          + '.' + std::to_string(this->minor)
          + '.' + std::to_string(this->patch);
        if (!this->label.empty()) {
            literal += '-' + this->label;
        }
        if (!this->build.empty()) {
            literal += '+' + this->build;
        }
        return literal;
    }
}
