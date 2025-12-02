#include "utils.hpp"

#include <fstream>
#include <string>

std::vector<std::string> load_file(std::filesystem::path path) {
    std::vector<std::string> file_lines;
    
    if (auto file = std::ifstream(path)) {
        std::string line;
        while (std::getline(file, line)) {
            file_lines.push_back(line);
        }
    }

    return file_lines;
}