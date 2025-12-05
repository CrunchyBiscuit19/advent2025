#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <queue>
#include <chrono>
#include <format>

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::vector<std::vector<long>> ranges; ranges.reserve(1 << 10);
    std::regex del("-");
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        if (line.size() == 0) break;
        std::vector<long> range; range.resize(2);
        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        for (int i = 0; it != end; it++) range[i++] = std::stol(*it);
        ranges.push_back(range);
    }
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        long tested = std::stol(line);
        for (auto range: ranges) {
            if (tested >= range[0] && tested <= range[1]) {
                std::cout << tested << std::endl;
                out++;
                break;
            }
        } 
    }
    file.close();

    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}