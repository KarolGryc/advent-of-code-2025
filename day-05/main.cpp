#include<vector>
#include<string>
#include<print>
#include "../utils/utils.hpp"

using IngredientID = uint64_t;

struct Range {
    IngredientID start;
    IngredientID end;
};

Range parseRange(const std::string& line) {
    size_t dashPos = line.find('-');
    
    if (dashPos == std::string::npos) {
        throw "Error";
    }
    
    IngredientID start = std::stoull(line.substr(0, dashPos));
    IngredientID end = std::stoull(line.substr(dashPos + 1));

    return {start, end};
}

auto parseIngredientInput(const std::vector<std::string>& lines) ->
std::pair<std::vector<Range>, std::vector<IngredientID>> {
    std::vector<Range> ranges;
    std::vector<IngredientID>  ids;   
    
    bool parsingRanges = true;
    for (const auto& line : lines) {
        if (line.size() == 0) {
            parsingRanges = false;
            continue;
        }

        if (parsingRanges) {
            ranges.push_back(parseRange(line));
        }
        else { // parse Id
            ids.push_back(std::stoull(line));
        }
    }

    return {ranges, ids};
}

bool isFresh(const std::vector<Range>& freshRange, IngredientID id) {
    for (auto range: freshRange) {
        if (id >= range.start && id <= range.end) {
            return true;
        }
    }
    return false;
}

int countFreshProducts(const std::vector<Range>& ranges, const std::vector<IngredientID>& ids) {
    int count = 0;
    for (auto id : ids) {
        count += isFresh(ranges, id);
    }

    return count;
}

std::vector<Range> mergeOverlappingRanges(std::vector<Range> ranges) {
    if (ranges.empty()) {
        return {};
    }
    
    std::sort(ranges.begin(), ranges.end(), [](auto a, auto b) { return a.start < b.start; });

    std::vector<Range> merged;
    Range mergedRange = ranges.front();
    for (int i = 1; i < ranges.size(); i++) {
        Range currRange = ranges[i];
        if (mergedRange.end >= currRange.start - 1) {
            mergedRange.end = std::max(currRange.end, mergedRange.end);
        }
        else {
            merged.push_back(mergedRange);
            mergedRange = currRange;
        }
    }
    merged.push_back(mergedRange);

    return merged;
}

uint64_t countRangesAllFreshIds(const std::vector<Range> ranges) {
    uint64_t cnt{};
    for (auto range : ranges) {
        uint64_t elNum = range.end - range.start + 1;
        cnt += elNum;
    }
    
    return cnt;
}

int main() {
    auto lines = loadFile("input.txt");
    auto [ranges, ids] = parseIngredientInput(lines);
    std::println("Fresh products: {}", countFreshProducts(ranges, ids));

    auto merged = mergeOverlappingRanges(ranges);
    std::println("Fresh products in ranges: {}", countRangesAllFreshIds(merged));
}