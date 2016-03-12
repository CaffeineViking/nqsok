#ifndef NQSOK_ROOT_HH
#define NQSOK_ROOT_HH

#include <vector>
#include <string>
#include <stdexcept>

namespace nq {
    class Root_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Root final {
    public:
        Root(const std::string&);
        const std::string& get_directory() const { return directory; }
        const std::vector<std::string>& get_packs() const { return packs; }

    private:
        std::string directory;
        std::vector<std::string> packs;
    };
}

#endif
