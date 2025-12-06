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

#define NUM_LINES 5
#define NUM_OPERANDS_PER_LINE 1000
//#define NUM_LINES 4
//#define NUM_OPERANDS_PER_LINE 4

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::regex del(" ");
    std::vector<std::vector<std::string>> worksheet; worksheet.reserve(NUM_LINES);
    while (std::getline(file, line)) {
        std::vector<std::string> wsLine; wsLine.reserve(NUM_OPERANDS_PER_LINE);

        std::sregex_token_iterator it(line.begin(), line.end(), del, -1);
        std::sregex_token_iterator end;
        for (; it != end; it++) {
            std::string operand = *it;
            operand.erase(remove_if(operand.begin(), operand.end(), isspace), operand.end());
            if (!operand.empty()) wsLine.emplace_back(operand);
        }

        worksheet.emplace_back(wsLine);
    }
    file.close();

    for (auto it = worksheet.begin(); it != worksheet.end(); it++) {
        for (auto it1 = it->begin(); it1 != it->end(); it1++) {
            std::cout << std::format("{} ", *it1);
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < NUM_OPERANDS_PER_LINE; i++) {
        long result = UINT64_MAX;
        char operation = worksheet[NUM_LINES - 1][i][0];
        for (int j = 0; j < NUM_LINES - 1; j++) {
            long operand = std::stol(worksheet[j][i]);
            if (result == UINT64_MAX) { 
                result = operand;
                continue;
            }
            std::cout << std::format("{} {} {} -> ", result, operation == '+' ? "+" : "*", operand);
            switch(operation) {
                case '+': 
                    result += operand;
                    break;
                case '*':
                    result *= operand;
                    break;
            }
        }
        std::cout << std::format("= {}", result) << std::endl;
        out += result;
    }

    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}