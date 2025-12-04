#include <print>
#include <vector>
#include <future>
#include <utility>

#include "../utils/utils.hpp"

template<typename T>
struct Grid {
    Grid(int x, int y) : content(y, std::vector<T>(x)) {}

    inline int ySize() const { return content.size(); }
    inline int xSize() const { return content[0].size(); }
    inline bool inBounds(int x, int y) const { return x >= 0 && x < xSize() && y >= 0 && y < ySize(); }

    std::vector<std::vector<T>> content;
};

Grid<bool> parseToPaperRollsGrid(const std::vector<std::string>& lines) {
    // Assumption that all rows have the same size
    const char paperRollSign = '@';
    int ySize = lines.size();
    int xSize = lines[0].size();

    Grid<bool> grid(xSize, ySize);

    for (int y = 0; y < ySize; y++) {
        auto& line = lines[y];
        
        for (int x = 0 ; x < xSize; x++) {
            grid.content[y][x] = lines[y][x] == paperRollSign; 
        }
    }

    return grid;
}

bool valAt(const Grid<bool> grid, int x, int y) {
    if (grid.inBounds(x, y)) {
        return grid.content[y][x];
    }

    return false;
}

int countNeighbours(const Grid<bool>& grid, int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            if (valAt(grid, x + dx, y + dy)) {
                count++;
            }
        }
    }

    return count;
}

int countAccessibleRolls(const Grid<bool>& grid) {
    int xSize = grid.xSize();
    int ySize = grid.ySize();
    int cnt = 0;
    
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            bool containsRoll = valAt(grid, x, y);
            if (!containsRoll) {
                continue;
            }
            
            cnt += countNeighbours(grid, x, y) < 4 ? 1 : 0;
        }
    }

    return cnt;
}

std::pair<int, Grid<bool>> countRemoveAccessible(const Grid<bool>& grid) {
    Grid<bool> buffer = grid;
    int xSize = grid.xSize();
    int ySize = grid.ySize();
    int cnt = 0;
    
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            bool containsRoll = valAt(grid, x, y);
            if (!containsRoll) {
                continue;
            }
            
            if (countNeighbours(grid, x, y) < 4) {
                buffer.content[y][x] = false;
                cnt++;
            }
        }
    }

    return {cnt, buffer};
}

std::pair<int, Grid<bool>> countRemoveAccessibleParallel(const Grid<bool>& grid){
    Grid<bool> buffer = grid;
    int xSize = grid.xSize();
    int ySize = grid.ySize();

    int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0) threadCount = 4;

    int rowsPerThread = (ySize + threadCount - 1) / threadCount;

    std::vector<std::future<int>> tasks;

    for (int t = 0; t < threadCount; t++) {
        int yStart = t * rowsPerThread;
        int yEnd   = std::min(yStart + rowsPerThread, ySize);

        if (yStart >= yEnd) break;

        tasks.push_back(std::async(std::launch::async,
            [&, yStart, yEnd]() -> int {

                int localCnt = 0;

                for (int y = yStart; y < yEnd; ++y) {
                    for (int x = 0; x < xSize; ++x) {

                        bool containsRoll = valAt(grid, x, y);
                        if (!containsRoll)
                            continue;

                        if (countNeighbours(grid, x, y) < 4) {
                            buffer.content[y][x] = false;
                            localCnt++;
                        }
                    }
                }

                return localCnt;
            }
        ));
    }

    int total = 0;
    for (auto& f : tasks)
        total += f.get();

    return {total, buffer};
}


int countWithReduce(Grid<bool> grid) {
    int totalCnt = 0;
    while (true) {
        auto [cnt, buffer] = countRemoveAccessible(grid);
        if (cnt == 0) {
            return totalCnt;
        }

        totalCnt += cnt;
        grid = buffer;
    }
}

int countWithReduceParallel(Grid<bool> grid) {
    int totalCnt = 0;
    while (true) {
        auto [cnt, buffer] = countRemoveAccessibleParallel(grid);
        if (cnt == 0) {
            return totalCnt;
        }

        totalCnt += cnt;
        grid = buffer;
    }
}

void partOneForFile(std::filesystem::path path) {
    auto lines = loadFile(path);
    auto parsedGrid = parseToPaperRollsGrid(lines);
    std::print("Result of part 1: {}\n", countAccessibleRolls(parsedGrid));
}

void partTwoForFile(std::filesystem::path path) {
    auto lines = loadFile(path);
    auto parsedGrid = parseToPaperRollsGrid(lines);
    std::print("Result of part 2: {}\n", countWithReduce(parsedGrid));
}

void partTwoParallel(std::filesystem::path path) {
    auto lines = loadFile(path);
    auto parsedGrid = parseToPaperRollsGrid(lines);
    std::print("Result of part 2: {}\n", countWithReduceParallel(parsedGrid));
}

int main() {
    partOneForFile("input.txt");
    // partTwoForFile("input.txt");
    partTwoParallel("input.txt");
}