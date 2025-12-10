#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <print>
#include <functional>
#include <cassert>

template <>
struct std::formatter<std::pair<long, long>> : std::formatter<std::string> {
    auto format(std::pair<long, long> p, format_context& ctx) const {
        return formatter<string>::format(std::format("[{}, {}]", p.first, p.second), ctx);
    }
};

long area(std::pair<long, long>& a, std::pair<long, long>& b) {
    return (std::abs(a.first - b.first) + 1) * (std::abs(a.second - b.second) + 1);  
}

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    long out = 0;
    std::vector<std::pair<long, long>> points;
    std::regex del(",");
    long maxRow = -1; long maxCol = -1;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        long col; long row;
        for (int i = 0; it != end; it++) {
            switch (i++) {
                case 0:
                    col = std::stol(*it);
                    maxCol = std::max(maxCol, col);
                    break;
                case 1:
                    row = std::stol(*it);
                    maxRow = std::max(maxRow, row);
                    break;
            }  
        } 
        points.emplace_back(col, row); 
    }
    file.close();

    // Get coordinates on opposite ends of a square (min to max in diagonal downwards), and calculate rectangle
    auto& minUp = *std::min_element(points.begin(), points.end(), [](std::pair<long, long>& a, std::pair<long, long>& b) {
        return std::pow(a.first, 2) + std::pow(a.second, 2) < std::pow(b.first, 2) + std::pow(b.second, 2);
    });
    auto& maxUp = *std::max_element(points.begin(), points.end(), [](std::pair<long, long>& a, std::pair<long, long>& b) {
        return std::pow(a.first, 2) + std::pow(a.second, 2) < std::pow(b.first, 2) + std::pow(b.second, 2);
    });
    long area1 = area(minUp, maxUp);
    std::println("Area of {} and {} is {}", minUp, maxUp, area1);

    // Get coordinates on opposite ends of a square (min to max in diagonal upwards), and calculate rectangle
    auto& minDown = *std::min_element(points.begin(), points.end(), [maxRow](std::pair<long, long>& a, std::pair<long, long>& b) {
        return std::pow(a.first, 2) + std::pow(std::abs(a.second - maxRow), 2) < std::pow(b.first, 2) + std::pow(std::abs(b.second - maxRow), 2);
    });
    auto& maxDown = *std::max_element(points.begin(), points.end(), [maxRow](std::pair<long, long>& a, std::pair<long, long>& b) {
        return std::pow(a.first, 2) + std::pow(std::abs(a.second - maxRow), 2) < std::pow(b.first, 2) + std::pow(std::abs(b.second - maxRow), 2);
    });
    long area2 = area(minDown, maxDown);
    std::println("Area of {} and {} is {}", minDown, maxDown, area2);

    std::println("Out {}", std::max(area1, area2));

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());

    return 0;
}