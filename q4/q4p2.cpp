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

#define NUM_LINES 137
//#define NUM_LINES 10
#define LINE_LENGTH NUM_LINES
#define UP -NUM_LINES
#define DOWN +NUM_LINES
#define LEFT -1
#define RIGHT +1
#define DUL (UP + LEFT)
#define DUR (UP + RIGHT)
#define DDL (DOWN + LEFT)
#define DDR (DOWN + RIGHT)
#define PAPER '@'
#define FREE '.'

int main() {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file("input");
    std::string line;
    if (!file.is_open()) return 0;

    // Answer
    long out = 0;
    std::string map = ""; map.reserve(NUM_LINES * NUM_LINES);
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end()); // Strip all whitespaces from the lines
        map.append(line);
    }
    file.close();
    std::vector<int> removePaperIndices; removePaperIndices.reserve(NUM_LINES * LINE_LENGTH);
    int localOut = 0;
    do {
        localOut = 0;
        for (int i = 0; i < map.size(); i++) {
            if (map[i] == FREE) continue;
            int free = 0; int paper = 0;
            if (i + UP < 0 || map[i + UP] == FREE) free++;
            if (i + DOWN > map.size() - 1 || map[i + DOWN] == FREE) free++;
            if (i + LEFT < i / LINE_LENGTH * LINE_LENGTH || map[i + LEFT] == FREE) free++;
            if (i + RIGHT > i / LINE_LENGTH * LINE_LENGTH + LINE_LENGTH - 1 || map[i + RIGHT] == FREE) free++;
            if (i < LINE_LENGTH || i % LINE_LENGTH == 0 || map[i + DUL] == FREE) free++;
            if (i < LINE_LENGTH || i % LINE_LENGTH == LINE_LENGTH - 1 || map[i + DUR] == FREE) free++;
            if (i >= (NUM_LINES - 1) * LINE_LENGTH || i % LINE_LENGTH == 0 || map[i + DDL] == FREE) free++;
            if (i >= (NUM_LINES - 1) * LINE_LENGTH || i % LINE_LENGTH == LINE_LENGTH - 1 || map[i + DDR] == FREE) free++;
            paper = 8 - free;
            if (paper < 4) {
                localOut++;
                removePaperIndices.push_back(i);
            }
        }
        for (int i = 0; i < removePaperIndices.size(); i++) {
            map[removePaperIndices[i]] = FREE; 
        }
        removePaperIndices.clear();
        out += localOut;
    } while (localOut > 0);
    std::cout << "Out " << out << std::endl;

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << duration.count() << " ms" << std::endl;

    return 0;
}