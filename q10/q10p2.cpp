#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <print>
#include <functional>
#include <cassert>

#define LONGEST_STATE 12

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Get all points
    long out = 0;
    std::regex tgtRegex("\\{(.*?)\\}");
    std::regex tgtDelimiter(",");
    std::regex edgeRegex("\\((.*?)\\)");
    std::regex edgeDelimiter(",");
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        
        // Parse target
        std::vector<long> tgt; tgt.resize(LONGEST_STATE); std::fill(tgt.begin(), tgt.end(), 0);
        std::smatch tgtMatch; std::regex_search(line, tgtMatch, tgtRegex);
        std::string tgtString = tgtMatch.str().substr(1, tgtMatch.str().size() - 2);
        std::sregex_token_iterator it(tgtString.begin(), tgtString.end(), tgtDelimiter, -1); std::sregex_token_iterator end;
        for (int i = 0; it != end; i++) {
            tgt[i] = std::stol(it->str());
            it++;
        }
        
        // Parse edges
        std::vector<std::set<long, std::greater<long>>> edgeSets; edgeSets.resize(LONGEST_STATE); 
        std::smatch edgeMatch;
        while (std::regex_search(line, edgeMatch, edgeRegex)) {
            long edge = 0;
            std::string edgeLights = edgeMatch.str().substr(1, edgeMatch.str().size() - 2);
            std::sregex_token_iterator it(edgeLights.begin(), edgeLights.end(), edgeDelimiter, -1); std::sregex_token_iterator end;
            std::vector<long> positions;
            for (int i = 0; it != end; i++) {
                long position = std::stol(it->str());
                positions.emplace_back(position);
                edge |= 1 << position;
                it++;
            }
            for (auto& position: positions) {
                edgeSets[position].insert(edge);
            }
            line = edgeMatch.suffix().str();
        }

        // DEBUG
        int a = 1;
        for (auto& edgeSet: edgeSets) {
            std::print("{}: ", a++);
            for (auto& edge: edgeSet) {
                std::print("{} ", edge);
            }
            std::println("");
        }

        // Keep subtracting until tgt is all zeroes
        while (true) {
            // Find largest element, largest element index (if multiple largest, get the first) O(1)
            const auto& maxIt = std::max_element(tgt.begin(), tgt.end());
            long maxIndex = std::distance(tgt.begin(), maxIt);
            long max = *maxIt;
            // Pick largest edge that contains 1 at maxIndex, and does not have 0 at any of the 1s
            long nextEdge = *edgeSets[maxIndex].begin();
            // Multiply the edge by either smallest integer at any of the 1s, or until there is a new largest or equal element


        }
    }
    file.close();
  
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());
    
    // Print result
    std::println("Out {}", out);

    return 0;
}