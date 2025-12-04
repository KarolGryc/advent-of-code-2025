#include <print>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <stdexcept>

#include "../utils/utils.hpp"

std::vector<int> string_to_int_vec(const std::string& str) {
    std::vector<int> res(str.size());
    for (int i = 0; i < str.size(); i++) {
        res[i] = std::stoi(std::string(1, str[i]));
    }

    return res;
}

std::vector<std::vector<int>> parseLines(const std::vector<std::string>& lines) {
    std::vector<std::vector<int>> parsedLines;
    for (const auto& line : lines) {
        auto parsed = string_to_int_vec(line);
        parsedLines.push_back(parsed);
    }

    return parsedLines;
}

int evalForTwoBatteries(const std::vector<int>& bank) {
    int max{};
    for (int i = 0; i < bank.size(); i++) {
        for (int j = i + 1; j < bank.size(); j++) {
            int val = bank[i] * 10 + bank[j];
            max = std::max(val, max);
        }
    }

    return max;
}

int evallAllLinesForTwoBatteries(const std::vector<std::vector<int>>& banks) {
    int sum{};
    for (const auto& bank : banks) {
        sum += evalForTwoBatteries(bank);
    }

    return sum;
}

int evalForNBatteries(const std::vector<int>& bank, int n) {
    if (n <= 0 || n > bank.size()) {
        throw std::string("Input error");
    }

    std::vector<int> solution(bank.end() - n, bank.end());
    std::vector<int> rest(bank.begin(), bank.end() - n);
    std::reverse(rest.begin(), rest.end());

    for (auto el: rest) {
        int tested = el;
        for (int i = 0; i < solution.size(); i++) {
            if (tested < solution[i]) {
                break;
            }
            
            std::swap(solution[i], tested);
        }
    }

    int num{};
    for (auto el : solution) {
        num *= 10;
        num += el;
    }

    return num;
}

int evalAllForNBatteries(const std::vector<std::vector<int>>& banks, int n) {
    int sum{};
    for (const auto& bank: banks) {
        sum += evalForNBatteries(bank, n);
    }

    return sum;
}

int main() {
    // test eval
    auto lines = loadFile("test.txt");
    auto parsedLines = parseLines(lines);
    std::print("Evaluated test: {}\n", evallAllLinesForTwoBatteries(parsedLines));
    std::print("Evalueated for 12 batteries: {}\n", evalAllForNBatteries(parsedLines, 12));

    // actual eval
    lines = loadFile("input.txt");
    parsedLines = parseLines(lines);
    std::print("Evaluated input.txt: {}\n", evallAllLinesForTwoBatteries(parsedLines));
    std::print("Evauluated input.txt for 12 batteries: {}\n", evalAllForNBatteries(parsedLines, 12));

    return 0;
}