#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>

int main() {
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    long sum = 0;
    std::regex del("-");
    std::vector<std::string> results; results.resize(2);

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
            if (i % 2 != 0) continue;
            long lower = leftStart;
            if (i != leftDigits) lower = pow(10, i / 2 - 1);
            long curr = 0;
            while (lower < pow(10, i / 2)) {
                curr = lower * pow(10, i / 2) + lower;
                if (curr >= leftLimit && curr <= rightLimit) localSum += curr;
                if (curr > rightLimit) break;
                lower++;
            }
            if (curr > rightLimit) break;
        }

        sum += localSum;
        std::cout << localSum << std::endl;
    }
    file.close();

    std::cout << sum << std::endl;

    return 0;
}