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

#define NUM_BATTERIES 12

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("test0");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        long constructed = 0; 
        int currIndex = 0;
        int digitsRemaining = NUM_BATTERIES;
        int windowSize = line.size() + 1 - digitsRemaining - currIndex; 

        for (int i = NUM_BATTERIES - 1; i >= 0; i--) {
            auto digitIt = std::max_element(line.begin() + currIndex, line.begin() + currIndex + windowSize);
            constructed += (*digitIt - '0') * pow(10, i);

            currIndex = std::distance(std::begin(line), digitIt) + 1;
            digitsRemaining--;
            windowSize = line.size() + 1 - digitsRemaining - currIndex;
            assert(currIndex + digitsRemaining + windowSize == line.size() + 1);
        }   

        std::cout << "Line Max " << constructed << std::endl;
        out += constructed;
    }
    file.close();
    std::cout << "Out Max " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}