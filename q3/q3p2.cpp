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
#include <cassert>

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("test1");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::string constructedInt = "            "; // 12 spaces for 12 integers
    while (std::getline(file, line)) {
        long lineMax = 0;

        // Store per-bank indices of each individual digit
        std::unordered_map<int, std::priority_queue<int, std::vector<int>, std::greater<int>>> bankIndices;
        for (int i = 0; i < line.size(); i++) { 
            int enteredBattery = line[i] - '0';
            bankIndices.try_emplace(enteredBattery);
            bankIndices.at(enteredBattery).push(i);
        }
        auto bankIndicesCopy = bankIndices;        
        
        // Test at each starting point
        for (int i = 0; i <= line.size() - 12; i++) {
            int constructedDigits = 0;
            bankIndicesCopy = bankIndices;
            // Initialize for each starting point
            constructedInt[0] = line[i];
            bankIndices.at(line[i] - '0').pop();
            bankIndicesCopy.at(line[i] - '0').pop();
            constructedDigits++;
            int windowStart = i + 1;
            int digitsRemaining = 10;
            int windowSize = line.size() - digitsRemaining - windowStart;

            // Pop from, adjust, and creating sliding window until windowSize == 1
            while (windowSize > 1 && constructedDigits != 12) {
                // Construct slidingWindow
                std::priority_queue<int> slidingWindow;
                for (int j = 0; j < windowSize; j++) slidingWindow.push(line[windowStart + j] - 48); // 'n' -> n
                 // Place max in slidingWindow into string
                constructedInt[12 - digitsRemaining - 1] = static_cast<char>(slidingWindow.top() + 48); // n -> 'n'
                constructedDigits++;
                // Adjust window settings for next iteration
                windowStart = bankIndicesCopy.at(slidingWindow.top()).top() + 1;
                
                digitsRemaining--;
                windowSize = line.size() - digitsRemaining - windowStart;
                // Pop the leftmost index of the biggest integer within the sliding window
                while (!slidingWindow.empty() && !bankIndicesCopy.at(slidingWindow.top()).empty() && bankIndicesCopy.at(slidingWindow.top()).top() < windowStart) {
                    bankIndicesCopy.at(slidingWindow.top()).pop();
                }
            }

            // Add rest after only 1 substring is left available
            if (constructedDigits < 12) {
                for (int j = windowStart; j < line.size(); j++) {
                    constructedInt[12 - digitsRemaining - 1] = line[j];
                    digitsRemaining--;
                }
            }

            // Update max of each line
            //std::cout << "Constructed " << constructedInt << std::endl;
            lineMax = std::max(lineMax, std::stol(constructedInt));
            
            constructedInt = "            ";
       }

       std::cout << "Line Max " << lineMax << std::endl;
       out += lineMax;
    }
    file.close();
    std::cout << "Out Max " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}