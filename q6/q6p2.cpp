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

//#define NUM_LINES 5
#define NUM_LINES 5

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long long out = 0;
    std::regex gap("([\\s]{2})");
    std::string worksheet; worksheet.reserve(12 << 1);
    int lineLength = 0;
    while (std::getline(file, line)) {
        worksheet.append(line);
        lineLength = line.size();
    }
    file.close();

    int numDigits = 0;
    std::vector<long long> operands; operands.reserve(12 << 1);
    for (int i = 0; i < lineLength; i++) {
        bool isColumnBreak = true;
        std::string operand; operand.reserve(NUM_LINES);
        // Identify the border
        for (int j = i; j < worksheet.size() - lineLength; j += lineLength) {
            if (worksheet[j] != ' ') isColumnBreak = false;
        }
        if (i % lineLength == lineLength) isColumnBreak = true;
        // If border, calculate results
        if (isColumnBreak) {
            char operation = worksheet[(i - numDigits) + (NUM_LINES - 1) * (lineLength)];
            long long result = (operation == '+') ? 0 : 1;
            for (int j = 0; j < operands.size(); j++) {
                switch (operation) {
                    case '+':
                        result += operands[j];
                        break;
                    case '*':
                        result *= operands[j];
                        break;
                }
            }
            out += result;
            numDigits = 0;
            operands.clear();
        // If not, gather the operands
        } else {
            numDigits++;
            for (int j = i; j < worksheet.size() - lineLength; j += lineLength) {
                if (worksheet[j] != ' ' && worksheet[j] <= '9' && worksheet[j] >= '0') operand.push_back(worksheet[j]);
            }
            operands.push_back(std::stol(operand));
        }
    }

    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}