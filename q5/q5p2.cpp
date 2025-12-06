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

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    auto cmp = [](std::pair<long, long> a, std::pair<long, long> b) { return a.first < b.first; };
    std::regex del("-");
    std::set<std::pair<long, long>, decltype(cmp)> ranges;
    std::pair<long, long> currRange; 
    std::pair<long, long> minRep; 
    std::pair<long, long> maxRep; 
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        if (line.size() == 0) break;

        // This entire algorithm hinges on the invariant that there are no overlapping ranges within the set
        
        // Build the current range
        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        for (int i = 0; i < 2; i++) {
            if (i == 0) currRange.first = std::stol(*it);
            else currRange.second = std::stol(*it);
            it++;
        }
        minRep.first = currRange.first; // It doesn't matter what minRep / maxRep second is, only using these as queries
        maxRep.first = currRange.second;

        // Get minimum and maximum iteators and values of new range after inserting current range
        long finalMin = currRange.first; long finalMax = currRange.second; 
        auto minIt = std::lower_bound(ranges.rbegin(), ranges.rend(), minRep, [](const std::pair<long, long>& a, const std::pair<long, long>& b) {
            return a.first > b.first;
        });
        if (minIt != ranges.rend() && currRange.first <= minIt->first + minIt->second) { 
            finalMin = minIt->first;
        }
        auto maxIt = std::lower_bound(ranges.rbegin(), ranges.rend(), maxRep, [](const std::pair<long, long>& a, const std::pair<long, long>& b) {
            return a.first > b.first;
        });
        if (maxIt != ranges.rend() && currRange.second <= maxIt->first + maxIt->second) {
            finalMax = maxIt->first + maxIt->second;
        }

        // Delete all ranges in between minimum and maximum iterators, including minIt and maxIt if present
        std::set<std::pair<long, long>, decltype(cmp)>::iterator travIt;
        if (minIt != ranges.rend() && currRange.first <= minIt->first + minIt->second) {
            travIt = ranges.find(std::pair<long, long>(minIt->first, 0)); // Re-find the range the minimum was in, with the iterator now in the forward position
        } else {
            travIt = ranges.upper_bound(minRep); // Find the immediate next range if the minimum isn't in a range 
        }
        while (travIt != ranges.end() && travIt->first <= finalMax) {
            travIt = ranges.erase(travIt);
        }
        
        // Insert new range into the set
        ranges.insert(std::pair<long, long>(finalMin, finalMax - finalMin));
    }
    file.close();

    for (auto it = ranges.begin(); it != ranges.end(); it++) out += (1 + it->second);
    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}