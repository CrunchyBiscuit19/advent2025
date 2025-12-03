#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <unordered_set>
#include <chrono>

void addSet(std::unordered_set<long>& localSet, int factor, int i, long leftLimit, long rightLimit) {
    long curr = pow(10, factor - 1);
    while (curr < pow(10, factor)) {
        long temp = curr;
        for (int j = factor; j < i; j += factor) temp += curr * pow(10, j);
        if (temp >= leftLimit && temp <= rightLimit) localSet.emplace(temp);
        if (temp > rightLimit) break;
        curr++;
    }   
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    long sum = 0;
    std::regex del("-");
    std::vector<std::string> results; results.resize(2);
    std::unordered_set<long> localSet;

    while (std::getline(file, line)) {
        long localSum = 0;

        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        int i = 0;
        while (it != end) {
            results[i++] = *it;
            ++it;
        }

        int leftDigits = results[0].size();
        long leftLimit = std::stol(results[0]);
        long leftStart = leftLimit / (pow(10, leftDigits / 2));
        
        int rightDigits = results[1].size();
        long rightLimit = std::stol(results[1]);
    
        results.clear();

        for (int i = leftDigits; i <= rightDigits; i++) {
            if (i == 1) continue;
            for (int j = 1; j <= 5; j++) if (i % j == 0 && i != j) addSet(localSet, j, i, leftLimit, rightLimit);
        }
        
        // Add all
        for (auto it = localSet.begin(); it != localSet.end(); it++) localSum += *it;

        sum += localSum;
        localSet.clear();
    }
    file.close();

    std::cout << sum << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}