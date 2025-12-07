#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <print>

#define LINE_LENGTH (142 - 1)
#define LINE_NUMS (143 - 1)
//#define LINE_LENGTH (16 - 1) 
//#define LINE_NUMS (17 - 1)

void pathfind(long source, std::string& map, std::unordered_set<long>& splitters) {
    long trav = source;
    // Travel until splitter
    while (trav <= map.size() && map[trav] != '^') {
        if (map[trav] != 'S') map[trav] = '|';
        trav += LINE_LENGTH;
    }
    // Stop if at end of map
    if (trav > map.size()) return;
    // Split and record new sources
    splitters.insert(trav);
    if (map[trav - 1] != '|') pathfind(trav - 1, map, splitters);
    if (map[trav + 1] != '|') pathfind(trav + 1, map, splitters);
}

int main(int argc, char *argv[]) {
    // Timer start
    auto start = std::chrono::high_resolution_clock::now();
    
    // Input handler
    std::ifstream file(argv[1]);
    std::string line;
    if (!file.is_open()) return 0;

    // Build map
    long out = 0;
    std::string map; map.reserve(LINE_LENGTH * LINE_LENGTH);
    std::unordered_set<long> splitters;
    while (std::getline(file, line)) {
        line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
        map.append(line);
    }
    file.close();
    
    // Find source
    long firstsourcePos = 0;
    for (int i = 0; i < LINE_LENGTH; i++) {
        if (map[i] == 'S') {
            firstsourcePos = i;
            break;
        }
    }

    // Pathfind
    pathfind(firstsourcePos, map, splitters);

    std::println("Out {}", splitters.size());

    // Timer end
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "μs" << std::endl;

    return 0;
}