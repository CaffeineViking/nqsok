#include "score.hh"
#include <fstream>
#include <iostream>

nq::Score::Score(const std::string& path)
            : file_path {path},
              player {"No Name"} {
    std::ifstream file {path};
    if (!file) return; // Will be created later.
    std::cout << "\nScore (parsing)..." << std::endl;
    std::cout << "Score(s) found '" << path << "'" << std::endl;
    std::cout << "File exists, parsing JSON..." << std::endl;

    Json::Value root; file >> root;
    player = root.get("player", "No Name").asString();
    Json::Value score_entries = root["entries"];
    if (!score_entries) throw Score_error{"Score error (#1) no entries!"};
    entries.reserve(score_entries.size());

    for (std::size_t i {0}; i < score_entries.size(); ++i) {
        Json::Value entry = score_entries[static_cast<int>(i)];
        if (entry.size() != 2) throw Score_error{"Score error (#2) missing data"};
        std::string level_path = entry[static_cast<int>(0)].asString();
        unsigned level_steps = entry[static_cast<int>(1)].asUInt();
        entries[level_path] = level_steps;
    }

    std::cout << "Found " << entries.size() << " score(s)" << std::endl;
    std::cout << "...done (Score)" << std::endl;
}

unsigned nq::Score::level(const std::string& level) const {
    if (entries.find(level) == entries.end()) return 0;
    return entries.at(level);
}

void nq::Score::apply(const std::string& level, unsigned value) {
    if (entries.find(level) == entries.end()) entries[level] = value;
    if (entries[level] > value) entries[level] = value;
}

void nq::Score::write() const {
    Json::Value root;
    root["player"] = player;
    int entry_counter {0}; // Stupid...
    for (const auto& entry : entries) {
        root["entries"][entry_counter][0] = entry.first;
        root["entries"][entry_counter][1] = entry.second;
        ++entry_counter; // Very stupid, damn iterators.
    }

    std::ofstream file {file_path};
    file << root; // Write to file.
}
