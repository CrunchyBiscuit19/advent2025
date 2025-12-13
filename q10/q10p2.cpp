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
#define INF_VALUE 1e18

enum class Flag {
    UNFINISHED,
    ALMOST,
    DONE,
    UNUSED
};

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
        
        // Parse target O(1)
        std::vector<long> tgt; tgt.resize(LONGEST_STATE); std::fill(tgt.begin(), tgt.end(), 0);
        std::smatch tgtMatch; std::regex_search(line, tgtMatch, tgtRegex);
        std::string tgtString = tgtMatch.str().substr(1, tgtMatch.str().size() - 2);
        long tgtSize = 0;
        std::sregex_token_iterator it(tgtString.begin(), tgtString.end(), tgtDelimiter, -1); std::sregex_token_iterator end;
        for (int i = 0; it != end; i++) {
            tgt[i] = std::stol(it->str());
            tgtSize++;
            it++;
        }
        
        // Parse edges O(E)
        std::vector<std::set<long, std::greater<long>>> edgeSets; edgeSets.resize(LONGEST_STATE); 
        std::unordered_map<long, std::vector<long>> edgeDigits;
        std::smatch edgeMatch;
        while (std::regex_search(line, edgeMatch, edgeRegex)) {
            long edge = 0;
            std::string edgeLights = edgeMatch.str().substr(1, edgeMatch.str().size() - 2);
            std::sregex_token_iterator it(edgeLights.begin(), edgeLights.end(), edgeDelimiter, -1); std::sregex_token_iterator end;
            std::vector<long> positions;
            for (int i = 0; it != end; i++) {
                long position = std::stol(it->str());
                positions.emplace_back(position);
                edge |= 1 << ((LONGEST_STATE - 1) - position);
                it++;
            }
            for (auto& position: positions) {
                edgeSets[position].insert(edge);
            }
            edgeDigits.emplace(edge, positions);
            line = edgeMatch.suffix().str();
        }

        // DEBUG
        int a = 0;
        for (auto& edgeSet: edgeSets) {
            std::print("{}: ", a++);
            for (auto& edge: edgeSet) {
                std::print("{} ", edge);
            }
            std::println("");
        }
        for (auto& edge: edgeDigits) {
            std::print("{}: ", edge.first);
            for (auto& digit: edge.second) {
                std::print("{} ", digit);
            }
            std::println("");
        }

        // Score setup
        std::vector<long> score; score.resize(LONGEST_STATE); std::fill(score.begin(), score.end(), 0);
        std::vector<Flag> flags; flags.resize(LONGEST_STATE); std::fill(flags.begin(), flags.end(), Flag::UNUSED);
        for (int i = 0; i < tgtSize; i++) flags[i] = Flag::UNFINISHED;

        // Increase score and track number of button presses O(k) where k is largest digit in tgt
        long presses = 0;
        while (true) {
            // Choose digit O(1)
            // 1. Prioritise UNFINISHED digits over ALMOST DIGITS
            // 2. Based on smallest rightmost digit in tgt, not score
            long chosenDigit = INF_VALUE;
            long chosenDigitValue = INF_VALUE;
            for (int i = 0; i < flags.size(); i++) {
                if (flags[i] == Flag::UNFINISHED && tgt[i] <= chosenDigitValue) {
                    chosenDigit = i;
                    chosenDigitValue = tgt[i];
                }
            }
            if (chosenDigit == INF_VALUE) {
                for (int i = 0; i < flags.size(); i++) {
                    if (flags[i] == Flag::ALMOST && tgt[i] <= chosenDigitValue) {
                        chosenDigit = i;
                        chosenDigitValue = tgt[i];
                    }
                }
            }
            assert(chosenDigit != INF_VALUE);

            // Pick largest edge that fit the criteria O(E)
            // 1. Must include chosen digit
            // 2. Must exclude ALMOST and DONE digits. If not possible, must exclude DONE digits
            long chosenEdge = INF_VALUE;
            for (auto edgeIt = edgeSets[chosenDigit].begin(); edgeIt != edgeSets[chosenDigit].end(); edgeIt++) {
                bool skip = false;
                for (long digit: edgeDigits.at(*edgeIt)) {
                    if (flags[digit] == Flag::ALMOST || flags[digit] == Flag::DONE) {
                        skip = true;
                        break;
                    }
                }
                if (skip) continue;
                chosenEdge = *edgeIt;
                break;
            }
            if (chosenEdge == INF_VALUE) {
                for (auto edgeIt = edgeSets[chosenDigit].begin(); edgeIt != edgeSets[chosenDigit].end(); edgeIt++) {
                    bool skip = false;
                    for (long digit: edgeDigits.at(*edgeIt)) {
                        if (flags[digit] == Flag::DONE) {
                            skip = true;
                            break;
                        }
                    }
                    if (skip) continue;
                    chosenEdge = *edgeIt;
                    break;
                }
            }
            assert(chosenEdge != INF_VALUE);
            
            // Find smallest difference in digits between tgt and current score, then multiply the edge by that, and add it to the score O(1)
            // 1. If UNFINISHED chosen digit, increase it to ALMOST
            // 2. If ALMOST chosen digit, increase it by 1 to DONE
            long smallestDifference = INF_VALUE;
            for (long edgeDigit: edgeDigits[chosenEdge]) {
                if (flags[edgeDigit] != Flag::UNUSED && flags[edgeDigit] != Flag::DONE) {
                    if (flags[edgeDigit] == Flag::UNFINISHED) {
                        smallestDifference = std::min(smallestDifference, tgt[edgeDigit] - 1 - score[edgeDigit]);
                    } else {
                        smallestDifference = std::min(smallestDifference, tgt[edgeDigit] - score[edgeDigit]);
                    }
                } 
            } 
            if (flags[chosenDigit] == Flag::UNFINISHED) smallestDifference = std::min(smallestDifference, tgt[chosenDigit] - 1 - score[chosenDigit]);
            
            // Increase press count and update score / flags
            presses += smallestDifference; 
            for (auto& edgeDigit: edgeDigits.at(chosenEdge)) {
                score[edgeDigit] += smallestDifference;
                if (score[edgeDigit] == tgt[edgeDigit] - 1) flags[edgeDigit] = Flag::ALMOST;
                else if (score[edgeDigit] == tgt[edgeDigit]) flags[edgeDigit] = Flag::DONE;
            }
            
            // DEBUG
            std::println("Chose Digit {} at {}", chosenDigitValue, chosenDigit);
            std::println("Chose Edge {} x {}", chosenEdge, smallestDifference);
            for (auto& digit: score) std::print("{} ", digit);
            std::println("");
            
            // Break if score equal to tgt, else repeat loop
            bool repeat = false;
            for (int i = 0; i < score.size(); i++) {
                if (score[i] != tgt[i]) {
                    repeat = true;
                    break;
                }
            }
            if (repeat) continue;
            break;
        }

        out += presses;
        std::println("{} presses", presses);

    }
    file.close();

    // 
  
    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::println("{}μs", duration.count());
    
    // Print result
    std::println("Out {}", out);

    return 0;
}