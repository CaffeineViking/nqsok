#ifndef NQSOK_PACK_HH
#define NQSOK_PACK_HH

#include <vector>
#include <string>
#include <stdexcept>

namespace nq {
    class Pack_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Pack final {
    public:
        Pack(const std::string&);
        const std::string& get_title() const { return title; }
        const std::string& get_description() const { return description; }
        const std::string& get_date() const { return date; }
        const std::string& get_version() const { return version; }
        const std::string& get_directory() const { return directory; }
        const std::vector<std::string>& get_levels() const { return levels; }

    private:
        std::string title;
        std::string description;
        std::string date, version;
        std::vector<std::string> levels;
        std::string directory;
    };
}

#endif
