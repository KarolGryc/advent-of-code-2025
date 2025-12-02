#include <print>
#include <vector>
#include <string>
#include <sstream>

#include "../utils/utils.hpp"

struct Range {
    int64_t start;
    int64_t end;
};

std::vector<Range> parseRangesLine(const std::string& line) {
    std::vector<Range> result;
    std::stringstream ss(line);

    std::string unparsedRange;
    while(std::getline(ss, unparsedRange, ',')) {
        size_t dashPos = unparsedRange.find('-');

        int64_t firstNum = std::stoll(unparsedRange.substr(0, dashPos));
        int64_t secondNum = std::stoll(unparsedRange.substr(dashPos + 1));

        result.push_back({firstNum,secondNum});
    }

    return result;
}

bool isNumberSus(int64_t num) {
    std::string numStr = std::to_string(num);
    size_t size = numStr.size();

    if (size % 2 != 0) {
        return false;
    }

    std::string firstHalf = numStr.substr(0, size / 2);
    std::string secondHalf = numStr.substr(size / 2);

    return firstHalf == secondHalf;
}

bool stringBuiltFromSequence(const std::string& str, const std::string& seq) {
    if (str.size() % seq.size() != 0) {
        return false;
    }

    for (int i = 0; i < str.size(); i += seq.size()) {
        std::string strSub = str.substr(i, seq.size());
        if (strSub != seq) {
            return false;
        }
    }

    return true;
}

bool isNumberSusSecondStar(int64_t num) {
    std::string numStr = std::to_string(num);

    size_t size = numStr.size();
    for (int checkedLen = 1; checkedLen <= size / 2; checkedLen++) {
        std::string seq = numStr.substr(0, checkedLen);
        if (stringBuiltFromSequence(numStr, seq)) {
            return true;
        }
    }

    return false;
}

int64_t evaluateRange(Range range, bool (*checkedFun)(int64_t)) {
    int64_t sum{};
    for (int64_t i = range.start; i <= range.end; i++) {
        if (checkedFun(i)) {
            sum += i;
        }
    }

    return sum;
}

int64_t sumSusIds(const std::vector<Range>& ranges, bool (*fun)(int64_t)) {
    int64_t sum{};
    for (auto range : ranges) {
        sum += evaluateRange(range, fun);
    }

    return sum;
}

void evalFile(std::filesystem::path path) {
    std::vector<std::string> inputLines = loadFile(path);
    std::vector<Range> ranges = parseRangesLine(inputLines[0]);
    std::print("Sus sum: {}\n", sumSusIds(ranges, isNumberSus));
}

void evalFileSecondStar(std::filesystem::path path) {
    std::vector<std::string> inputLines = loadFile(path);
    std::vector<Range> ranges = parseRangesLine(inputLines[0]);
    std::print("Sus sum second star: {}\n", sumSusIds(ranges, isNumberSusSecondStar));
}

int main() {
    evalFile("input.txt");
    evalFileSecondStar("input.txt");
    return 0;
}