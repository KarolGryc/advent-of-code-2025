#include <fstream>
#include <print>
#include <vector>
#include <string>
#include <exception>

#include "../utils/utils.hpp"

int parseRotation(const std::string& command) {
    if (command.empty()) {
        throw std::invalid_argument("Rotation command can't be empty!");
    }

    int sign = toupper(command[0]) == 'L' ? -1 : 1;
    int val = std::stoi(command.substr(1));

    return sign * val;
}

int getCode(const std::vector<std::string>& taskInput) {
    int currPos = 50;
    int dialSize = 100;
    int zeroCount = 0;

    for (const auto& command: taskInput) {
        int rotation = parseRotation(command) % dialSize;
        int newVal = currPos + rotation;
        currPos = newVal >= 0 ? (newVal % dialSize) : (newVal + dialSize);
        
        if (currPos == 0) {
            zeroCount++;
        }
    }

    return zeroCount;
}

int getCodeMethod0x434C49434B(const std::vector<std::string>& taskInput) {
    int currPos = 50;
    int dialSize = 100;
    int zeroCount = 0;

    for (const auto& command: taskInput) {
        int rotation = parseRotation(command);
        int wholeRotations = std::abs(rotation / dialSize);
        int modRotation = rotation % dialSize;

        int newVal = currPos + modRotation;

        if (currPos == 0) {
            if (abs(rotation) > 100) {
                zeroCount += wholeRotations;
            }
        }
        else if (newVal > dialSize || newVal < 0) {
            zeroCount += wholeRotations + 1;
        }
        else {
            zeroCount += wholeRotations;
        }

        currPos = newVal >= 0 ? (newVal % dialSize) : (newVal + dialSize);

        if (currPos == 0) {
            zeroCount++;
        }
    }

    return zeroCount;
}

void executeFile(std::filesystem::path path, int (*fun)(const std::vector<std::string>&)) {
    std::vector<std::string> lines = load_file(path);
    int result = fun(lines);
    std::print("The code is: {0}\n", result);
}

int main() {
    // executeFile("small_input.txt", getCode);
    // executeFile("large_input.txt", getCode);

    executeFile("small_input.txt", getCodeMethod0x434C49434B);
    executeFile("large_input.txt", getCodeMethod0x434C49434B);
}