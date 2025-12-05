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
    std::ifstream file("test1");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    auto cmp = [](std::pair<long, long> a, std::pair<long, long> b) { return a.first < b.first; };
    std::regex del("-");
    std::set<std::pair<long, long>, decltype(cmp)> ranges;
    std::pair<long, long> currRange; 
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        if (line.size() == 0) break;
        
        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        for (int i = 0; it != end; it++) {
            if (i == 0) currRange.first = std::stol(*it);
            else currRange.second = std::stol(*it);
        }

        bool minInRange = true; bool maxInRange = true;
        auto minIt = ranges.lower_bound(currRange);
        if (minIt == ranges.end() || currRange.first >= minIt->first - minIt->second) minInRange = false;
        auto maxIt = ranges.lower_bound(currRange);
        if (maxIt == ranges.end() || currRange.first >= maxIt->first - maxIt->second) maxInRange = false;

        if (!minInRange && !maxInRange) {
            std::pair<long, long> newMinRange = currRange;
            newMinRange.second = newMinRange.second - newMinRange.first;
            ranges.insert(newMinRange);
            std::pair<long, long> newMaxRange = currRange;
            newMaxRange.first = currRange.second;
            newMaxRange.second = newMinRange.second;
            ranges.insert(newMaxRange);
        }
        if (minInRange) {
            long decreased = minInRange ? minIt->first : currRange.first;
            auto newMinRange = *minIt;
            auto newMaxRange = *maxIt;
            newMaxRange.second = maxIt->first - decreased;
            newMinRange.first = decreased;
            newMinRange.second = maxIt->second;

            
        }
        if (maxInRange) {
            long increased = maxInRange ? maxIt->first : currRange.second;
            auto newMinRange = *minIt;
            auto newMaxRange = *maxIt;
            newMinRange.second = increased - minIt->first;
            newMaxRange.first = increased;
            newMaxRange.second = minIt->second;
            ranges.erase(minIt);
            ranges.erase(maxIt);
            ranges.insert(newMinRange);
            ranges.insert(newMaxRange);
            currRange.second = increased;
        }
    }
    file.close();

    for (auto it = rangesStart.begin(); it != rangesStart.end(); it++) std::cout << std::format("{}-{}", it->first, it->second) << std::endl;
    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}