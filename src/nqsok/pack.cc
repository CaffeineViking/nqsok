#include "pack.hh"

#include <fstream>
#include <iostream>
#include <json/json.h>

nq::Pack::Pack(const std::string& path) {
    std::cout << "\nPack (parsing)..." << std::endl;
    std::cout << "Path is '" << path << "'" << std::endl;

    std::ifstream file {path};
    if (!file) throw Pack_error{"Pack error (#1) no such file!"};
    std::cout << "File exists, parsing JSON..." << std::endl;

    Json::Value root; file >> root;
    title = root.get("title", "Untitled").asString();
    std::cout << "Title is '" << title << "'\n";
    description = root.get("description", "").asString();
    std::cout << "Description is '" << description << "'\n";
    date = root.get("date", "").asString();
    std::cout << "Creation date was '" << date << "'\n";
    version = root.get("version", "").asString();
    std::cout << "Version is '" << version << "'\n";

    Json::Value directory_value = root["directory"];
    if (!directory_value) throw Pack_error{"Pack error (#2) no directory specified!"};
    directory = directory_value.asString();
    std::cout << "Directory is '" << directory << "'" << std::endl;

    Json::Value levels_value = root["levels"];
    if (!levels_value) throw Pack_error{"Pack error (#3) at least one level must exist in pack!"};
    levels.reserve(levels_value.size()); // Pre-allocate some memory...
    for (std::size_t i {0}; i < levels_value.size(); ++i) {
        levels.push_back(levels_value[static_cast<int>(i)].asString());
    }

    std::cout << "Found " << levels_value.size() << " level(s)" << std::endl;
    std::cout << "...done (Pack)" << std::endl;
}
