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

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("test1");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::multiset<long> bankSet;
    std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>> bankIndices;
    std::priority_queue<int, std::vector<int>, std::greater<int>> testIndices;
    while (std::getline(file, line)) {
        long localMax = 0;
        for (int i = 0; i < line.size() - 1; i++) {
            long enteredBattery = static_cast<long>(line[i] - '0');
            bankSet.insert(enteredBattery);
            bankIndices.try_emplace(enteredBattery);
            bankIndices.at(enteredBattery).push(i);
        }
        auto bankIndicesCopy = bankIndices;
        for (int i = 0; i < line.size() - 12; i++) {
            long testMax = 0;

            // Get leading digit index
            long removedBattery = static_cast<long>(line[i] - '0');
            int removedBatteryIndex = bankIndices.at(removedBattery).top(); 
            bankIndices.at(removedBattery).pop();
            bankIndicesCopy.at(removedBattery).pop();
            testIndices.push(removedBatteryIndex);
            bankSet.erase(bankSet.find(removedBattery));

            // Get other 11 digits indices
            auto testTrav = std::prev(bankSet.end());
            for (int j = 0; j < 11; j++) {
                testIndices.push(bankIndicesCopy.at(*testTrav).top()); 
                bankIndicesCopy.at(*testTrav).pop();
                testTrav = std::prev(testTrav);
            }

            // Create local test number
            for (int j = 11; !testIndices.empty(); j--) {
                testMax += (line[testIndices.top()] - '0') * std::pow(10, j); 
                testIndices.pop();
            }
            localMax = std::max(localMax, testMax);
        }
        std::cout << "LocalMax " << localMax << std::endl;
        out += localMax;
        bankSet.clear();
        bankIndices.clear();
    }
    file.close();
    std::cout << "Max " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}