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

        const std::string& get_player() const { return player; }
        const std::string& get_file() const { return file_path; }
        void set_player(const std::string& name) { player = name; }

    private:
        std::string file_path;
        std::string player {"No Name"};
        std::unordered_map<std::string,
                           unsigned> entries;
    };
}

#endif
