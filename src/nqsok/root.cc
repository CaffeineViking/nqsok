#include "root.hh"

#include <fstream>
#include <iostream>
#include <json/json.h>

nq::Root::Root(const std::string& path) {
    std::cout << "\nRoot (parsing)..." << std::endl;
    std::cout << "Root is '" << path << "'" << std::endl;

    std::ifstream file {path};
    if (!file) throw Root_error{"Root error (#1) no such file!"};
    std::cout << "File exists, parsing JSON..." << std::endl;

    Json::Value root; file >> root;
    Json::Value directory_value {root["directory"]};
    if (!directory_value) throw Root_error{"Root error (#2) no directory specified!"};
    directory = directory_value.asString();

    Json::Value packs_value {root["packs"]};
    if (!packs_value) throw Root_error{"Root error (#3) at least one pack is needed!"};
    packs.reserve(packs_value.size()); // Pre-allocate buffer...
    for (std::size_t i {0}; i < packs_value.size(); ++i) {
        packs.push_back(packs_value[static_cast<int>(i)].asString());
    }

    std::cout << "Found " << packs.size() << " pack(s)" << std::endl;
    std::cout << "...done (Root)" << std::endl;
}
