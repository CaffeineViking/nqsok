#ifndef NQSOK_SCORE_HH
#define NQSOK_SCORE_HH

#include <stdexcept>
#include <json/json.h>
#include <unordered_map>
#include <string>

namespace nq {
    class Score_error final : public std::runtime_error {
    public: using std::runtime_error::runtime_error; };

    class Score final {
    public:
        Score(const std::string&);
        unsigned level(const std::string&) const;
        void apply(const std::string&, unsigned);
        void write() const; // To the file path.

    private:
        std::string file_path;
        std::unordered_map<std::string,
                           unsigned> entries;
    };
}

#endif
