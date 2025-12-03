#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <chrono>

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::multiset<long> bankSet;
    while (std::getline(file, line)) {
        long localMax = 0;
        for (int i = 0; i < line.size(); i++) {
            long enteredBattery = static_cast<long>(line[i] - '0');
            bankSet.insert(enteredBattery);
        }
        for (int i = 0; i < line.size() - 1; i++) {
            long removedBattery = static_cast<long>(line[i] - '0');
            bankSet.erase(bankSet.find(removedBattery));
            long innerMax = *std::prev(bankSet.end());
            localMax = std::max(localMax, removedBattery * 10 + innerMax);
        }
        out += localMax;
        bankSet.clear();
    }
    file.close();
    std::cout << "Max " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}